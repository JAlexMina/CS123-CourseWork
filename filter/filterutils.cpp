#include "filterutils.h"
#include <algorithm>
#include "math.h"
#include <cstring>
#include <iostream>



namespace FilterUtils {

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}




void normalize(std::vector<float> &vec)
{
    float sz = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        sz += vec[i];
    }

    float invSz = 1 / sz;

    for (int i = 0; i < vec.size(); i++)
    {
        vec[i] = vec[i] * invSz;
    }
}




void Convolve1D(RGBA* data, int width, int height, const std::vector<float> &kernel1, const std::vector<float> &kernel2) {


    RGBA* results = new RGBA[width*height];
    RGBA* intermediate = new RGBA[width*height];



    int dim1 = kernel1.size();
    int halfDim1 = floor(dim1/2);

    int dim2 = kernel2.size();
    int halfDim2 = floor(dim2/2);



    //convoling the columns
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;



            float red_acc = 0.f;
            float green_acc = 0.f;
            float blue_acc = 0.f;



                //convolve
                for (int i = 0; i < dim1 ; i++)
                {

                        int pixCol = (c - halfDim1 + i);
                        int pi = r  * width + pixCol;

                        if(pixCol > 0 && pixCol < width - 1)
                        {
                            red_acc   += data[pi].r/255.f * (kernel1[i]);
                            green_acc += data[pi].g/255.f * (kernel1[i]);
                            blue_acc  += data[pi].b/255.f * (kernel1[i]);
                        }
                        else
                        {

                            pi = r  * width + c;
                            red_acc   += data[pi].r/255.f * (kernel1[i]);
                            green_acc += data[pi].g/255.f * (kernel1[i]);
                            blue_acc  += data[pi].b/255.f * (kernel1[i]);
                        }

                }

            intermediate[centerIndex] = RGBA(REAL2byte(red_acc), REAL2byte(green_acc), REAL2byte(blue_acc));


        }
    }

      //convoling the rows
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;


            float red_acc2 = 0.f;
            float green_acc2 = 0.f;
            float blue_acc2 = 0.f;


            //convolve
            for (int i = 0; i < dim2 ; i++)
            {
                    int pixRow = (r - halfDim2 + i);
                    int pi = pixRow  * width + c;

                    if(pixRow > 0 && pixRow < height - 1)
                    {
                        red_acc2   += intermediate[pi].r/255.f * (kernel2[i]);
                        green_acc2 += intermediate[pi].g/255.f * (kernel2[i]);
                        blue_acc2  += intermediate[pi].b/255.f * (kernel2[i]);

                    }
                    else
                    {

                        pi = r  * width + c;

                        red_acc2   += intermediate[pi].r/255.f * (kernel2[i]);
                        green_acc2 += intermediate[pi].g/255.f * (kernel2[i]);
                        blue_acc2  += intermediate[pi].b/255.f * (kernel2[i]);
                    }

            }

            results[centerIndex] = RGBA(REAL2byte(red_acc2), REAL2byte(green_acc2), REAL2byte(blue_acc2));
        }
    }
    memcpy(data, results, height*width*sizeof(RGBA));
    delete[] results;
    delete[] intermediate;



}


void convolve1DMarquee(Canvas2D *canvas, const std::vector<float> &kernel1, const std::vector<float> &kernel2, bool sharp)
{
    //Marquee/////////////////////////////////////////////////////////////////////////////////
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
    //Marque///////////////////////////////////////////////////////////////////////////////////



    //convolving the filter
    if (!sharp)
    {
        Convolve1D(dataa, newWidth, newHeight, kernel1, kernel2);
    }
    else
    {
        Convolve2D(dataa, newWidth, newHeight, kernel1);
    }

//        canvas->resize(newWidth, newHeight);

//        for(int i = 0; i < newWidth*newHeight; i++)
//        {
//            canvas->data()[i] = dataa[i];
//        }



    startY = canvas->marqueeStart().y();
    startX = canvas->marqueeStart().x();
    stopX = canvas->marqueeStop().x();
    stopY = canvas->marqueeStop().y();

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




    topLeft = startY * canvas->width() + startX;
    topRight = startY * canvas->width() + stopX;
    botLeft = stopY * canvas->width() + startX;
    botRight = stopY * canvas->width() + stopX;


    index = 0;




    while(topRight != botRight)
    {
        while(topLeft != topRight)
        {
            canvas->data()[topLeft] = dataa[index];
            index++;
            startX++;
            topLeft = startY * canvas->width() + startX;
        }
        startX = canvas->marqueeStart().x();
        startY++;
        topLeft = startY * canvas->width() + startX;
        topRight = startY * canvas->width() + stopX;
    }

    delete[] dataa;
}



float clamp (float val, float lower, float upper)
{
    if(val < lower)
    {
        return lower;
    }

    if(val > upper)
    {
        return upper;
    }

    return val;
}






void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel) {

    RGBA* results = new RGBA[width*height];



    int dim = sqrt(kernel.size());
    int halfDim = floor(dim*0.5);






    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;



            float red_acc = 0.f;
            float green_acc = 0.f;
            float blue_acc = 0.f;





                for (int row = 0; row < dim ; row++)
                {
                    for (int col = 0; col < dim ; col++)
                    {
                        int pixRow = (r - halfDim + row);
                        int pixCol = (c - halfDim + col);
                        int pi = pixRow  * width + pixCol;
                        int ki = row * dim + col;


                        if(!(pixRow < 0 || pixCol < 0 || pixRow > height - 1 || pixCol > width - 1))
                        {
                            red_acc   += data[pi].r/255.f * (kernel[ki]);
                            green_acc += data[pi].g/255.f * (kernel[ki]);
                            blue_acc  += data[pi].b/255.f * (kernel[ki]);
                        }
                        else
                        {
                            int pi = r  * width + c;
                            red_acc   += data[pi].r/255.f * (kernel[ki]);
                            green_acc += data[pi].g/255.f * (kernel[ki]);
                            blue_acc  += data[pi].b/255.f * (kernel[ki]);
                        }




                    }

                }





            results[centerIndex] = RGBA(REAL2byte(red_acc), REAL2byte(green_acc), REAL2byte(blue_acc));


        }
    }



    memcpy(data, results, height*width*sizeof(RGBA));
    delete[] results;

}



}
