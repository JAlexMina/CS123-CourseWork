/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"
#include "Canvas2D.h"
#include <cmath>

ConstantBrush::ConstantBrush(int radius)
    : Brush(radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}



void ConstantBrush::paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend) {

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
                       pix[getIndex(std::min(std::max((centeredY-col), 0), height), std::min(std::max((centeredX-row), 0), width), width)] =
                               alphaBlending(color, pix[getIndex(std::min(std::max((centeredY-col), 0), height), std::min(std::max((centeredX-row), 0), width), width)], m_mask[row+col]);
                   }
                   else
                   {
                       pix[getIndex(std::min(std::max((centeredY-col), 0), height), std::min(std::max((centeredX-row), 0), width), width)] = color;
                   }


            }


        }


    }


}



void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    for(int i = 0; i < (getRadius()*2)* (getRadius()*2); i++)
    {
        m_mask.push_back(1.0f);
    }

}


