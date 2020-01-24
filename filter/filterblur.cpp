#include "filterblur.h"
#include "filterutils.h"
#include <math.h>









FilterBlur::FilterBlur(int blurRadius)
{


    int dim = (2*blurRadius + 1);
    m_kernel.resize(dim);
    float first = 1.f;
    int index = 0;
    for (int col = 0; col < dim; col++)
    {
        m_kernel[index] = first;

        index++;
        if(col >= floor(dim*0.5))
        {
            first--;
        }
        else
        {
            first++;

        }
    }



    FilterUtils::normalize(m_kernel);


}

FilterBlur::~FilterBlur()
{
}

void FilterBlur::apply(Canvas2D *canvas) {



    if(canvas->marqueeStart() != canvas->marqueeStop())
    {
        FilterUtils::convolve1DMarquee(canvas, m_kernel, m_kernel, false);
    }
    else
    {
        FilterUtils::Convolve1D(canvas->data(), canvas->width(), canvas->height(), m_kernel, m_kernel);
    }

}
