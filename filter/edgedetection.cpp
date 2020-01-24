#include "edgedetection.h"
#include "filter/Filter.h"
#include "filter/FilterGray.h"
#include "filter/filterutils.h"
#include <math.h>
#include <QDebug>

#include <algorithm>



EdgeDetection::EdgeDetection(float sensitivity) :
    m_sen(sensitivity)
{



}

EdgeDetection::~EdgeDetection()
{}






void EdgeDetection::apply(Canvas2D *canvas)
{
    std::unique_ptr<Filter> filter;
    filter = std::unique_ptr<FilterGray>(new FilterGray());

    filter->apply(canvas);

    std::vector<float> sobelONE = {1, 0, -1};
    std::vector<float> sobelTWO = {1, 2, 1};
    const int siz = canvas->width() * canvas->height();




    //.... other code



    if(canvas->marqueeStart() != canvas->marqueeStop())
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
        int difSiz = newWidth*newHeight;

        RGBA* dataa = new RGBA[newWidth*newHeight];
        RGBA* dataaH = new RGBA[newWidth*newHeight];
        size_t index = 0;

        while(topRight != botRight)
        {
            while(topLeft != topRight)
            {
                dataa[index] = canvas->data()[topLeft];
                dataaH[index] = canvas->data()[topLeft];
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
        FilterUtils::Convolve1D(dataa, newWidth, newHeight, sobelONE, sobelTWO);
        FilterUtils::Convolve1D(dataaH, newWidth, newHeight, sobelTWO, sobelONE);

        for (int i = 0; i < difSiz; i++) {

            dataa[i].r = FilterUtils::clamp(sqrt((dataa[i].r*dataa[i].r) + (dataaH[i].r * dataaH[i].r)) * m_sen, 0.f, 255.f);
            dataa[i].g = FilterUtils::clamp(sqrt((dataa[i].g*dataa[i].g) + (dataaH[i].g * dataaH[i].g)) * m_sen, 0.f, 255.f);
            dataa[i].b = FilterUtils::clamp(sqrt((dataa[i].b*dataa[i].b) + (dataaH[i].b * dataaH[i].b)) * m_sen, 0.f, 255.f);



        }



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
        delete[] dataaH;
    }
    else
    {

        //RGBA datUH[siz];
        RGBA* datUH = new RGBA[siz];
        memcpy(datUH, canvas->data(), siz * sizeof(RGBA));
        //RGBA datUHoh[siz];
        RGBA* datUHoh = new RGBA[siz];
        memcpy(datUHoh, canvas->data(), siz * sizeof(RGBA));

        //Gx
        FilterUtils::Convolve1D(datUH, canvas->width(), canvas->height(), sobelONE, sobelTWO);
        //Gy
        FilterUtils::Convolve1D(datUHoh, canvas->width(), canvas->height(), sobelTWO, sobelONE);





        //G = √G^2x + G^2y
        for (int i = 0; i < siz; i++) {

            canvas->data()[i].r = FilterUtils::clamp(sqrt((datUH[i].r*datUH[i].r) + (datUHoh[i].r * datUHoh[i].r)) * m_sen, 0.f, 255.f);
            canvas->data()[i].g = FilterUtils::clamp(sqrt((datUH[i].g*datUH[i].g) + (datUHoh[i].g * datUHoh[i].g)) * m_sen, 0.f, 255.f);
            canvas->data()[i].b = FilterUtils::clamp(sqrt((datUH[i].b*datUH[i].b) + (datUHoh[i].b * datUHoh[i].b)) * m_sen, 0.f, 255.f);




        }

        delete[] datUH;
        delete[] datUHoh;
    }





}
