#ifndef SpecialBrush_Header
#define SpecialBrush_Header

#include "QuadraticBrush.h"

/**
 * @class SmudgeBrush
 *
 * Smudge brush, which smudges color values across the canvas as the mouse is dragged.
 */
class SmudgeBrush : public QuadraticBrush {

public:
    SmudgeBrush(int radius);
    virtual ~SmudgeBrush();

    virtual void brushDown(int x, int y, Canvas2D *canvas);
    virtual void brushDragged(int x, int y, Canvas2D *canvas);
    virtual void brushUp(int x, int y, Canvas2D *canvas);
    void paint(int x, int y, int width, int height, RGBA* pix, RGBA color, bool alphaBlend);
    RGBA alphaBlending(RGBA new_Color, RGBA old_Color, float m);

protected:
    // Constructs the mask for this brush.
    void makeMask();
    RGBA mixEm(RGBA oldColor, RGBA pickedUpCOlor);


    //! Copy paint in the bounding box of the brush from canvas to m_paint
    void pickUpPaint(int x, int y, Canvas2D* canvas);
    std::vector<RGBA> m_pickUp;
    std::vector<RGBA> m_hold;


};

#endif
