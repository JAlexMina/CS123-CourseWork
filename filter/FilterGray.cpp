#include "FilterGray.h"

unsigned char RGBAToGray(const RGBA &pixel) {
    // TODO: Task 5

    return (0.299*pixel.r + 0.587*pixel.g + 0.114*pixel.b);

}

FilterGray::~FilterGray()
{
}

void FilterGray::apply(Canvas2D *canvas) {
    int width = canvas->width();


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
                //canvas->data()[topLeft]; // <-----

                unsigned char pixel;
                pixel = RGBAToGray(canvas->data()[topLeft]);

                canvas->data()[topLeft].r = pixel;
                canvas->data()[topLeft].g = pixel;
                canvas->data()[topLeft].b = pixel;



                index++;
                startX++;
                topLeft = startY * canvas->width() + startX;
            }
            startX = canvas->marqueeStart().x();
            startY++;
            topLeft = startY * canvas->width() + startX;
            topRight = startY * canvas->width() + stopX;
        }
    }
    else
    {


        RGBA* current_pixel = nullptr;

        /* Initialize the first pixel of the first row */
        RGBA* current_row = canvas->data();

        RGBA* data = canvas->data();

        size_t currentIndex = 0;

        for (int r = 0; r < canvas->height(); r++) {
            current_pixel = current_row;
            currentIndex = r * width;

            for (int c = 0; c < canvas->width(); c++) {
                // TODO: Task 4
                unsigned char pixel;
                pixel = RGBAToGray(*current_pixel);




                // TODO: Task 6

                current_pixel->r = pixel;
                current_pixel->g = pixel;
                current_pixel->b = pixel;


                /* Advance to the next pixel */
                current_pixel++;
                currentIndex++;
            }
            current_row += width;
        }
    }
}

