 /**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>
#include <iostream>

QuadraticBrush::QuadraticBrush(int radius)
    : Brush(radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}



void QuadraticBrush::paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend) {

    //makeMask();



    int centeredX = x+(this->getRadius());
    int centeredY = y+(this->getRadius());
    int cols = 1+(this->getRadius())*2;
    int rows = 1+(this->getRadius())*2;




    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if(dist(col, row, getRadius(), getRadius(), getRadius()) <= getRadius())
            {
                //printf("%f\n", m_mask[mC]);




                   if(alphaBlend)
                   {
                       pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)] =
                               alphaBlending(color, pix[getIndex(std::min(std::max((centeredY-row), 0), height), std::min(std::max((centeredX-col), 0), width), width)], m_mask[getIndex(row, col, (getRadius()*2))]);
                   }
                   else
                   {
                       pix[getIndex(std::min(std::max((centeredY-col), 0), height), std::min(std::max((centeredX-row), 0), width), width)] = color;
                   }


            }


        }


    }


}


void QuadraticBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...




    int cols = 1+(getRadius()*2);
    int rows = 1+(getRadius()*2);


    int n = (1+(getRadius()*2)) * (1+(getRadius()*2));
    m_mask.reserve(n);






    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {


           float d =(dist(col, row, getRadius(), getRadius(), getRadius()));

           m_mask[getIndex(row, col, (getRadius()*2))] = (1 - (d/getRadius())) * (1 - (d/getRadius()));
        }

    }





}


