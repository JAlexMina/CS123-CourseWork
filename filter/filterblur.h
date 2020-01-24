#ifndef FILTERBLUR_H
#define FILTERBLUR_H

#include "Filter.h"


class FilterBlur : public Filter {

public:
    FilterBlur(int blurRadius);
    virtual ~FilterBlur() override;

    virtual void apply(Canvas2D *canvas) override;

private:
    std::vector<float> m_kernel;
};

#endif // FILTERBLUR_H
