#ifndef __BRUSH_H__    //header guards
#define __BRUSH_H__

#include <vector>

#include "RGBA.h"


class Canvas2D;

/**
 * @class Brush
 *
 * The superclass for all brushes.
 */
class Brush {
public:
    // Constructor for brush.  Pass it the initial color, flow and radius.
    Brush(int radius);

    // Destructor for brush.  Clean up all brush allocated resources.
    virtual ~Brush();

    RGBA getRGBA() const;
    int getAlpha() const;
    int getBlue() const;
    int getRed() const;
    int getGreen() const;
    int getRadius() const;

    void setAlpha(int alpha);
    void setBlue(int blue);
    void setGreen(int green);
    void setRed(int red);
    void setRGBA(const RGBA &rgba);
    void setRadius(int radius);
    int getIndex(int row, int col, int col_number);

    virtual void brushDown(int x, int y, Canvas2D* canvas) = 0;
    virtual void brushDragged(int x, int y, Canvas2D* canvas);
    virtual void brushUp(int x, int y, Canvas2D* canvas) = 0;
    virtual void paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend);
    virtual RGBA alphaBlending(RGBA new_Color, RGBA old_Color, float m);
    float dist(int x1, int y1, int x2, int y2, int r);



protected:
    // Pure virtual function that will create the mask distribution.
    // To be be implemented in subclasses.
    virtual void makeMask() = 0;

    // C++ coding convention is to put m_ in front of member variables
    RGBA m_color;
    std::vector<float> m_mask;
    int m_radius;


};


#endif // __BRUSH_H__

