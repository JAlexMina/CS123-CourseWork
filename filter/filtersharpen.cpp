#include "filtersharpen.h"
#include "filterutils.h"
#include <math.h>

FilterSharpen::FilterSharpen(int blurRadius)
{
    //Making a 2 dimensional triangle filter
    if(blurRadius > 7)
    {
        blurRadius = 7;
    }

    int dim = (2*blurRadius + 1);
    m_kernel.resize(dim*dim);
    float first = -0.1f;
    float mid = 0.f;
    float out = -0.1;
    int index = 0;

    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            m_kernel[index] = out;
            mid += out;
            index++;
            if(col == floor(dim*0.5)-1 && row == floor(dim*0.5))
            {
                out = abs(mid*2)+1;
                first = first - 0.1f;
            }
            else if(col >= floor(dim*0.5))
            {
                first = first + 0.1f;
                out = first;
            }
            else
            {
                first = first - 0.1f;
                out = first;
            }
        }
        if(!(row >= floor(dim*0.5)))
        {
            first = first- 0.2f;
            out = first;

        }
    }




}

FilterSharpen::~FilterSharpen()
{}


void FilterSharpen::apply(Canvas2D *canvas)
{












    const int siz = canvas->width() * canvas->height();
    RGBA* datUH = new RGBA[siz];
    memcpy(datUH, canvas->data(), siz * sizeof(RGBA));



    if(canvas->marqueeStart() != canvas->marqueeStop())
    {
        FilterUtils::convolve1DMarquee(canvas, m_kernel, m_kernel, true);
    }
    else
    {
        FilterUtils::Convolve2D(canvas->data(), canvas->width(), canvas->height(), m_kernel);
    }







    delete[] datUH;







}
