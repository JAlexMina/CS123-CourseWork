#ifndef FILTERUTILS_H
#define FILTERUTILS_H

#include "RGBA.h"
#include <vector>
#include "Canvas2D.h"

namespace FilterUtils {

inline unsigned char REAL2byte(float f);
void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel);
void Convolve1D(RGBA* data, int width, int height, const std::vector<float> &kernel1, const std::vector<float> &kernel2);
float clamp (float val, float lower, float upper);


void convolve1DMarquee(Canvas2D *canvas, const std::vector<float> &kernel1, const std::vector<float> &kernel2, bool sharp);

void normalize(std::vector<float> &vec);

}

#endif // FILTERUTILS_H
