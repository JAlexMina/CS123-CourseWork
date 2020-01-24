#ifndef QuadraticBrush_Header
#define QuadraticBrush_Header

#include "Brush.h"


/**
 * @class QuadraticBrush
 *
 * Quadratic brush, whose mask has a falloff of 1/r^2.
 */
class QuadraticBrush
    : public Brush
{

public:
    QuadraticBrush(int radius);
    virtual ~QuadraticBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas) {}
    virtual void brushUp(int x, int y, Canvas2D *canvas) {}
    void paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend);

protected:
    void makeMask(); // Constructs the mask for this brush.
};

#endif
