#ifndef CONE_IMPLICIT_H
#define CONE_IMPLICIT_H


#include "ImplicitShape.h"

class Cone_Implicit
{
public:
    Cone_Implicit();
    float Intersect(const Ray &ray);
    int findT(glm::vec3 p, glm::vec3 d, float *t1, float *t2);
    glm::vec3 intersectNormal(glm::vec3 intersection);
    UV getUV(glm::vec4 intersection);

private:
    float t;
};

#endif // CONE_IMPLICIT_H
