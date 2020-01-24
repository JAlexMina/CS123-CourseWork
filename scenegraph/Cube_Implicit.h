#ifndef CUBE_IMPLICIT_H
#define CUBE_IMPLICIT_H


#include "ImplicitShape.h"


class Cube_Implicit
{
public:
    Cube_Implicit();



    float Intersect(const Ray &r);
    glm::vec3 intersectNormal(glm::vec3 intersection);
    UV getUV(glm::vec4 intersection, int width, int height);


};

#endif // CUBE_IMPLICIT_H
