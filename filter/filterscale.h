#ifndef FILTERSCALE_H
#define FILTERSCALE_H

#include "Filter.h"


class FilterScale : public Filter
{
public:
    FilterScale(float scaleFactorX, float scaleFactorY);


    virtual ~FilterScale() override;

    virtual void apply(Canvas2D *canvas) override;

    void ConvolveX(const std::vector<float> &kernel1, float scaleFactor, int type, Canvas2D* canvas);
    void ConvolveY(const std::vector<float> &kernel1, float scaleFactor, int type, Canvas2D* canvas);
    double hprime(int k, double a, int pi);
    double g (double x, double a);


    enum ScaleType {
        UP,
        DOWN
    };

private:
    float m_scaleFactorX;
    float m_scaleFactorY;

    int m_scaleTYPEX;
    int m_scaleTYPEY;

    std::vector<float> m_xKernel;
    std::vector<float> m_yKernel;





};

#endif // FILTERSCALE_H
