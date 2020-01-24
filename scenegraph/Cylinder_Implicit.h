#ifndef CYLINDER_IMPLICIT_H
#define CYLINDER_IMPLICIT_H


#include "ImplicitShape.h"

class Cylinder_Implicit
{
public:
    Cylinder_Implicit();

    float Intersect(const Ray &ray);
    glm::vec3 intersectNormal(glm::vec3 intersection);
    UV getUV(glm::vec4 intersection);
};

#endif // CYLINDER_IMPLICIT_H
