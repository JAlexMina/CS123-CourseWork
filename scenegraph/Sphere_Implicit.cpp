#include "Sphere_Implicit.h"
#include <math.h>
#include <bits/stdc++.h>


Sphere_Implicit::Sphere_Implicit(float r) :
    m_radius(r)
{

}


UV Sphere_Implicit::getUV(glm::vec4 intersection)
{
    intersection = glm::normalize(intersection);
    UV toReturn;
    float u;
    float v;

    float theta = atan2(intersection.z, intersection.x);

    if(theta < 0)
    {
        u = (-theta)/(2.f*PI);
    }
    else
    {
        u = 1-(theta/(2.f*PI));
    }


    float phi = asin(intersection.y/1.f);



    v = asin(intersection.y) / PI + 0.5f;




    if(std::abs(v-0) <= EPSILON  || std::abs(v-1) <= EPSILON )
    {
        u = 0.5;
    }


    toReturn.U = u;
    toReturn.V = v;



    return toReturn;
}



float Sphere_Implicit::Intersect(const Ray &ray)
{

    glm::vec3 P = ray.orig;
    glm::vec3 d = ray.dir;



    float a = d.x * d.x + d.y * d.y + d.z * d.z;
    float b = 2.0 * (d.x * P.x) + 2.0 * (d.y * P.y) + 2.0 * (d.z * P.z);
    float c = P.x * P.x + P.y * P.y + P.z * P.z - 0.25;

    if (ImplicitShape::checkDiscriminant(a, b, c) == false)
    {
        return -1;
    }

    float t = ImplicitShape::quadraticFormula(a, b, c);
    return t;



}




glm::vec3 Sphere_Implicit::intersectNormal(glm::vec3 intersection)
{
    glm::vec3 normalVector = glm::normalize(intersection);
    return normalVector;
}











