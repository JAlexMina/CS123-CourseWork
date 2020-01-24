#ifndef SPHERE_IMPLICIT_H
#define SPHERE_IMPLICIT_H




#include "ImplicitShape.h"







class Sphere_Implicit
{
public:
    Sphere_Implicit(float r);


    float Intersect(const Ray &ray);

    

    glm::vec3 intersectNormal(glm::vec3 intersection);
    UV getUV(glm::vec4 intersection);



private:

    float m_radius;
    float t;



};

#endif // SPHERE_IMPLICIT_H
