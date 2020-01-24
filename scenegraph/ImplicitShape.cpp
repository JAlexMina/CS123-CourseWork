#include "ImplicitShape.h"

ImplicitShape::ImplicitShape()
{

}



ImplicitShape::~ImplicitShape()
{

}

float ImplicitShape::quadraticFormula(float a, float b, float c)
{

    float smallestNonNegt = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
    if (smallestNonNegt < 0.0) {
        smallestNonNegt = (-b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
    }
    if (smallestNonNegt < 0.0) {
        return -1;
    }
    return smallestNonNegt;
}

bool ImplicitShape::checkDiscriminant(float a, float b, float c)
{

    float discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0) {
        return false;
    }
    return true;
}
