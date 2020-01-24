#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H

#include "Filter.h"


class EdgeDetection : public Filter
{
public:
    EdgeDetection(float sensitivity);
    virtual ~EdgeDetection() override;


    virtual void apply(Canvas2D *canvas) override;

private:
    std::vector<float> m_kernel;
    float m_sen;
};

#endif // EDGEDETECTION_H
