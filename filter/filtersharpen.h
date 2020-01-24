#ifndef FILTERSHARPEN_H
#define FILTERSHARPEN_H


#include "Filter.h"




class FilterSharpen : public Filter
{
public:
    FilterSharpen(int blurRadius);
    virtual ~FilterSharpen() override;

    virtual void apply(Canvas2D *canvas) override;

private:
    std::vector<float> m_kernel;
};

#endif // FILTERSHARPEN_H
