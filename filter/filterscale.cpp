#include "filterscale.h"
#include "filterutils.h"
#include <math.h>

FilterScale::FilterScale(float scaleFactorX, float scaleFactorY) :
    m_scaleFactorX(scaleFactorX),
    m_scaleFactorY(scaleFactorY)
{

    if(m_scaleFactorX >= 1)
    {
        m_scaleTYPEX = UP;
        m_xKernel = {0.25f, 0.5f, 0.25f};
    }
    else
    {
        m_scaleTYPEX = DOWN;

        int dim = (2*(1/scaleFactorX) + 1);
        m_xKernel.resize(dim);
        float first = 1.f;
        int index = 0;
        for (int col = 0; col < dim; col++)
        {
            m_xKernel[index] = first;

            index++;
            if(col >= floor(dim*0.5))
            {
                first--;
            }
            else
            {
                first++;

            }
        }



        FilterUtils::normalize(m_xKernel);


    }


    if(m_scaleFactorY >= 1)
    {
        m_scaleTYPEY = UP;
        m_yKernel = {0.25f, 0.5f, 0.25f};
    }
    else
    {
        m_scaleTYPEY = DOWN;

        int dim = (2*(1/scaleFactorX) + 1);
        m_yKernel.resize(dim);
        float first = 1.f;
        int index = 0;
        for (int col = 0; col < dim; col++)
        {
            m_yKernel[index] = first;

            index++;
            if(col >= floor(dim*0.5))
            {
                first--;
            }
            else
            {
                first++;

            }
        }



        FilterUtils::normalize(m_yKernel);
    }

}


FilterScale::~FilterScale()
{
}


double FilterScale::g (double x, double a)
{
    double radius;
    if (a < 1)
    {
        radius = 1.0/a;
    }
    else
    {
        radius = 1.0;
    }

    if((x < -radius) || (x > radius))
    {
        return 0;
    }
    else
    {
        return (1 - fabs(x)/ radius);
    }
}

double FilterScale::hprime(int k, double a, int pi) {
    double sum = 0, weights_sum = 0;
    int left, right;
    float support;
    float center= k/a + (1-a)/(2*a);
    support = (a > 1) ? 1 : 1/a; //ternary operator
    left = ceil(center - support);
    right = floor(center + support);

    for (int i = left; i <= right; i++) {
        sum += g(i - center, a) * pi;
        weights_sum += g(i - center, a);
    }
    return (sum/weights_sum);
}





void FilterScale::apply(Canvas2D *canvas)
{


    //Marquee/////////////////////////////////////////////////////////////////////////////////
    if(canvas->marqueeStart() != canvas->marqueeStop())
    {


        int startY = canvas->marqueeStart().y();
        int startX = canvas->marqueeStart().x();
        int stopX = canvas->marqueeStop().x();
        int stopY = canvas->marqueeStop().y();

        if(startX > stopX)
        {
            int hold = stopX;
            stopX = startX;
            startX = hold;
        }

        if(startY > stopY)
        {
            int hold = stopY;
            stopY = startY;
            startY = hold;
        }

        if (stopX > canvas->width())
        {
            stopX = canvas->width();
        }
        if (stopX < 0)
        {
            stopX = 0;
        }
        if (stopY > canvas->height())
        {
            stopY = canvas->height();
        }
        if (stopY < 0)
        {
            stopY = 0;
        }

        size_t topLeft = startY * canvas->width() + startX;
        size_t topRight = startY * canvas->width() + stopX;
        size_t botLeft = stopY * canvas->width() + startX;
        size_t botRight = stopY * canvas->width() + stopX;

        int newWidth  = stopX - startX;
        int newHeight = stopY - startY;

        RGBA* dataa = new RGBA[newWidth*newHeight];
        size_t index = 0;

        while(topRight != botRight)
        {
            while(topLeft != topRight)
            {
                dataa[index] = canvas->data()[topLeft];
                index++;
                startX++;
                topLeft = startY * canvas->width() + startX;
            }
            startX = canvas->marqueeStart().x();
            startY++;
            topLeft = startY * canvas->width() + startX;
            topRight = startY * canvas->width() + stopX;
        }



                canvas->resize(newWidth, newHeight);

                for(int i = 0; i < newWidth*newHeight; i++)
                {
                    canvas->data()[i] = dataa[i];
                }


        delete[] dataa;




    }
    //Marque///////////////////////////////////////////////////////////////////////////////////









    switch (m_scaleTYPEX)
    {
        case UP:
        {
            //scale x up
            ConvolveX(m_xKernel, m_scaleFactorX, UP, canvas);
            break;
        }

        case DOWN:
        {
            //scale x down
            ConvolveX(m_xKernel, m_scaleFactorX, DOWN, canvas);
            break;
        }
    }




    switch (m_scaleTYPEY)
    {
        case UP:
        {
            //scale y up
            ConvolveY(m_yKernel, m_scaleFactorY, UP, canvas);
            break;
        }

        case DOWN:
        {
            //scale y down
            ConvolveY(m_yKernel, m_scaleFactorY, DOWN, canvas);
            break;
        }
    }

}



void FilterScale::ConvolveY(const std::vector<float> &kernel1, float scaleFactor, int type, Canvas2D* canvas) {


    if(scaleFactor != 1.f)
    {

        RGBA* results;
        float siz = 0;


        siz = canvas->width()*(canvas->height()*scaleFactor);
        results = new RGBA[static_cast<int>(siz)];



        int dim1 = kernel1.size();
        int halfDim1 = floor(dim1/2);


        int newHeight = canvas->height()*scaleFactor;

          //convoling the rows
        for (int r = 0; r < newHeight; r++) {
            for (int c = 0; c < canvas->width(); c++) {
                size_t centerIndex = r * canvas->width() + c;



                float red_acc2 = 0.f;
                float green_acc2 = 0.f;
                float blue_acc2 = 0.f;


                //convolve
                for (int i = 0; i < dim1 ; i++)
                {
                        int pixRow = (static_cast<int>(ceil(static_cast<float>(r)*(1/scaleFactor))) - halfDim1 + i);

                        int pi = pixRow  * canvas->width() + c;


                        if(pixRow > 0 && pixRow < canvas->height() - 1)
                        {
//                            red_acc2   += canvas->data()[pi].r/255.f * hprime(i, scaleFactor, pi);
//                            green_acc2 += canvas->data()[pi].g/255.f * hprime(i, scaleFactor, pi);
//                            blue_acc2  += canvas->data()[pi].b/255.f * hprime(i, scaleFactor, pi);
                            red_acc2   += canvas->data()[pi].r/255.f * (kernel1[i]);
                            green_acc2 += canvas->data()[pi].g/255.f * (kernel1[i]);
                            blue_acc2  += canvas->data()[pi].b/255.f * (kernel1[i]);

                        }
                        else
                        {

                            pi = static_cast<int>(ceil(static_cast<float>(r))*(1.f/scaleFactor))  * canvas->width() + c;
                            red_acc2   += canvas->data()[pi].r/255.f * (kernel1[i]);
                            green_acc2 += canvas->data()[pi].g/255.f * (kernel1[i]);
                            blue_acc2  += canvas->data()[pi].b/255.f * (kernel1[i]);
//                            red_acc2   += canvas->data()[pi].r/255.f * hprime(i, scaleFactor, pi);
//                            green_acc2 += canvas->data()[pi].g/255.f * hprime(i, scaleFactor, pi);
//                            blue_acc2  += canvas->data()[pi].b/255.f * hprime(i, scaleFactor, pi);
                        }
                }

                results[centerIndex] = RGBA(FilterUtils::REAL2byte(red_acc2), FilterUtils::REAL2byte(green_acc2), FilterUtils::REAL2byte(blue_acc2));
                //results[centerIndex] = hprime(i, scaleFactor, pi);
            }
        }



        canvas->resize(canvas->width(), newHeight);

        memcpy(canvas->data(), results, newHeight*canvas->width()*sizeof(RGBA));
        delete[] results;
    }

}



void FilterScale::ConvolveX(const std::vector<float> &kernel1, float scaleFactor, int type, Canvas2D* canvas) {


    if(scaleFactor != 1.f)
    {

        float siz = (canvas->width()*scaleFactor)*canvas->height();

        RGBA* results = new RGBA[static_cast<int>(siz)];



        int dim1 = kernel1.size();
        int halfDim1 = floor(dim1/2);

        int newWidth = canvas->width()*scaleFactor;



        //convoling the columns
        for (int r = 0; r < canvas->height(); r++) {
            for (int c = 0; c < newWidth; c++) {
                size_t centerIndex = r * newWidth + c;



                float red_acc = 0.f;
                float green_acc = 0.f;
                float blue_acc = 0.f;



                    //convolve
                    for (int i = 0; i < dim1 ; i++)
                    {


                            int pixCol = ((c*(1/scaleFactor)) - halfDim1 + i);
                            int pi = r  * canvas->width() + pixCol;

                            if(pixCol > 0 && pixCol < canvas->width() - 1)
                            {
                                red_acc   += canvas->data()[pi].r/255.f * (kernel1[i]);
                                green_acc += canvas->data()[pi].g/255.f * (kernel1[i]);
                                blue_acc  += canvas->data()[pi].b/255.f * (kernel1[i]);

                            }
                            else
                            {

                                pi = r  * canvas->width() + (static_cast<int>(ceil(static_cast<float>(c))*(1.f/scaleFactor)));
                                red_acc   += canvas->data()[pi].r/255.f * (kernel1[i]);
                                green_acc += canvas->data()[pi].g/255.f * (kernel1[i]);
                                blue_acc  += canvas->data()[pi].b/255.f * (kernel1[i]);
                            }
                    }

                results[centerIndex] = RGBA(FilterUtils::REAL2byte(red_acc), FilterUtils::REAL2byte(green_acc), FilterUtils::REAL2byte(blue_acc));


            }
        }




        canvas->resize(newWidth, canvas->height());

        memcpy(canvas->data(), results, newWidth*canvas->height()*sizeof(RGBA));
        delete[] results;
    }




}
