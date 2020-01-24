/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"
#include "filter/Filter.h"
#include "filter/filterblur.h"
#include "filter/filterscale.h"
#include "filter/edgedetection.h"
#include "filter/filtersharpen.h"


#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"



//#include <ctime>
//#include <time.h>

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    bType(settings.brushType),
    constB(new ConstantBrush(settings.brushRadius)),
    linB(new LinearBrush(settings.brushRadius)),
    quadB(new QuadraticBrush(settings.brushRadius)),
    smudB(new SmudgeBrush(settings.brushRadius)),
    alphaBlend(true),
    pix(data()),
    b(new ConstantBrush(settings.brushRadius))


{


}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    bType = settings.brushType;



    switch (bType)
    {
          case 0: {
                //Do Something (Constant Brush)
                b = constB;
                break;
          }

          case 1: {
                //Do Something (Linear Brush)
                b = linB;
                break;
          }
          case 2: {
                //Do Something (Quadratic Brush)
                b = quadB;
                break;
          }
          case 3: {
                //Do Something (Smudge Brush)
                b = smudB;
                break;
          }
          case 4:
                //Do Something (Special 1 Brush)

                break;

          case 5: {
                //Do Something (Special 2 Brush)

                break;
          }
          default: {
                //Do Something (Something Went Wrong)
                break;
          }

    }


    b->setRed(settings.brushColor.r);
    b->setGreen(settings.brushColor.g);
    b->setBlue(settings.brushColor.b);
    b->setAlpha(settings.brushColor.a);
    b->setRadius(settings.brushRadius);
    //b->makeMask();


}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************





void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

    RGBA* pix = data();



    if(bType == 3)
    {
        b->brushDown(x, y, this);
        //b->brushDragged(x, y, this);
    }

        b->paint(x, y, width(), height(), pix, settings.brushColor, alphaBlend);







    this->update();

}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    pix = data();

    if(bType == 3)
    {
        b->brushDragged(x, y, this);
        //b->brushDown(x, y, this);
    }
    else
    {
        b->paint(x, y, width(), height(), pix, settings.brushColor, alphaBlend);
    }



    this->update();

}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    if(bType == 3)
    {
        b->brushUp(x, y, this);
    }
    this->update();
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.



        std::unique_ptr<Filter> filter;

            // TODO: Task 2
            switch (settings.filterType)
            {
                case FILTER_BLUR:
                {
                    filter = std::unique_ptr<FilterBlur>(new FilterBlur(settings.blurRadius));
                    break;
                }

                case FILTER_EDGE_DETECT:
                {
                    filter = std::unique_ptr<EdgeDetection>(new EdgeDetection(settings.edgeDetectSensitivity));
                    break;
                }

                case FILTER_ROTATE:
                {
                    filter = std::unique_ptr<FilterBlur>(new FilterBlur(settings.blurRadius));
                    break;
                }

                case FILTER_SCALE:
                {
                    //Canvas2D::resize((Canvas2D::width()*settings.scaleX), (Canvas2D::height()*settings.scaleY));
                    filter = std::unique_ptr<FilterScale>(new FilterScale(settings.scaleX, settings.scaleY));
                    break;
                }

                case FILTER_SPECIAL_1:
                {
                    filter = std::unique_ptr<FilterSharpen>(new FilterSharpen(settings.blurRadius));
                    break;
                }
                case FILTER_SPECIAL_2:
                {
                    filter = std::unique_ptr<FilterBlur>(new FilterBlur(settings.blurRadius));
                    break;
                }
                case FILTER_SPECIAL_3:
                {
                    filter = std::unique_ptr<FilterBlur>(new FilterBlur(settings.blurRadius));
                    break;
                }
            }

            // TODO: Task 3

            filter->apply(this);

            this->update();


}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);    
}

void Canvas2D::renderImage(Camera *camera, int width, int height) {

    resize(width, height);


    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.



        m_rayScene->renderScene(this, camera);
        this->update();


    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




