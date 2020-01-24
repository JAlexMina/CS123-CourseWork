#ifndef LinearBrush_Header
#define LinearBrush_Header


#include "Brush.h"

/**
 * @class LinearBrush
 *
 * Linear brush, whose mask has linear falloff.
 */
class LinearBrush
    : public Brush
{

public:
    LinearBrush(int radius);
    virtual ~LinearBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas) {}
    virtual void brushUp(int x, int y, Canvas2D *canvas) {}
    void paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend);

protected:
    void makeMask(); // Constructs the mask for this brush.
};

#endif
