/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"
#include "Settings.h"
#include "Canvas2D.h"

#include <iostream>
SmudgeBrush::SmudgeBrush(int radius) :
    QuadraticBrush(radius)

{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!









    /////////////////////////////////////////////////////////

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //    

    int cols = 1+(getRadius()*2);
    int rows = 1+(getRadius()*2);


    int n = (1+(getRadius()*2)) * (1+(getRadius()*2));
    m_mask.reserve(n);






    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

            if(dist(col, row, getRadius(), getRadius(), getRadius()) <= getRadius())
            {


                float d =(dist(col, row, getRadius(), getRadius(), getRadius()));

                m_mask[getIndex(row, col, (getRadius()*2)+1)] = (1 - (d/getRadius())) * (1 - (d/getRadius()));
            }
        }

    }




}






void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    int width = canvas->width();
    int height = canvas->height();
    RGBA *pix = canvas->data();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //


    int n = (1+(getRadius()*2)) * (1+(getRadius()*2));
    m_pickUp.reserve(n);



    int centeredX = x+(this->getRadius());
    int centeredY = y+(this->getRadius());
    int cols = 1+(this->getRadius())*2;
    int rows = 1+(this->getRadius())*2;


    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {

            if(dist(col, row, getRadius(), getRadius(), getRadius()) <= getRadius())
            {


                m_pickUp[getIndex(row, col, (getRadius()*2)+1)] = pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)];
            }

        }





    }









}







//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    int width = canvas->width();
    int height = canvas->height();
    RGBA *pix = canvas->data();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //


    int n = (1+(getRadius()*2)) * (1+(getRadius()*2));
    m_pickUp.reserve(n);



    int centeredX = x+(this->getRadius());
    int centeredY = y+(this->getRadius());
    int cols = 1+(this->getRadius())*2;
    int rows = 1+(this->getRadius())*2;


    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if(dist(col, row, getRadius(), getRadius(), getRadius()) <= getRadius())
            {



                    m_pickUp[getIndex(row, col, (getRadius()*2)+1)] = mixEm(pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)], m_pickUp[getIndex(row, col, (getRadius()*2)+1)]);





            }

        }





    }





    //std::vector<float> m_mask;



}







void SmudgeBrush::brushDragged(int x, int y, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    // now pick up paint again...


    //paint(x, y, canvas->width(), canvas->height(), canvas->data(), settings.brushColor, true);


    pickUpPaint(x, y, canvas);



    paint(x, y, canvas->width(), canvas->height(), canvas->data(), RGBA(0,0,0,0), true);


}





RGBA SmudgeBrush::mixEm(RGBA oldColor, RGBA pickedUpCOlor)
{

    return RGBA( ((pickedUpCOlor.r) * 0.6 + (oldColor.r) * 0.4),
                 ((pickedUpCOlor.g) * 0.6 + (oldColor.g) * 0.4),
                 ((pickedUpCOlor.b) * 0.6 + (oldColor.b) * 0.4),
                 ((pickedUpCOlor.a) * 0.6 + (oldColor.a) * 0.4)  );



}





void SmudgeBrush::paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend) {



    int centeredX = x+(this->getRadius());
    int centeredY = y+(this->getRadius());
    int cols = 1+(this->getRadius())*2;
    int rows = 1+(this->getRadius())*2;






    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if(dist(col, row, getRadius(), getRadius(), getRadius()) <= getRadius())
            {





                    //m_hold = m_pickUp;

                    //std::cout << pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)].r << std::endl;
                    //printf("%d\n", 1);


                   if(!alphaBlend)
                   {
                       pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)] =
                               alphaBlending(m_pickUp[getIndex(row, col, (getRadius()*2)+1)], pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)], abs(1 - m_mask[getIndex(row, col, (getRadius()*2))]));
                   }
                   else
                   {
                     pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)] =  m_pickUp[getIndex(row, col, (getRadius()*2)+1)];
                   }


            }


        }


    }


}



RGBA SmudgeBrush::alphaBlending(RGBA new_Color, RGBA old_Color, float m) {


    float a = 1.0f;





    return RGBA( (new_Color.r * m * a) + (old_Color.r * (1-a*m)),
                 (new_Color.g * m * a) + (old_Color.g * (1-a*m)),
                 (new_Color.b * m * a) + (old_Color.b * (1-a*m)),
                                                         a);
}


