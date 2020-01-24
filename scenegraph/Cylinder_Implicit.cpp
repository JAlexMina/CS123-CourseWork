#include "Cylinder_Implicit.h"
#include <math.h>
#include <bits/stdc++.h>


Cylinder_Implicit::Cylinder_Implicit()
{

}


UV Cylinder_Implicit::getUV(glm::vec4 intersection)
{
    UV toReturn;
    float u;
    float v;



    if(std::abs(intersection.y+0.5f) <= EPSILON)
    {
       /* use y = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = (intersection.x + 0.5f);
        v = 1-(intersection.z + 0.5f);

    }
    else if(std::abs(intersection.y-0.5f) <= EPSILON)
    {
       /* use y = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = (intersection.x + 0.5f);
        v = (intersection.z + 0.5f);

    }
    else
    {
        float theta = atan2(intersection.z, intersection.x);

        if(theta < 0)
        {
            u = (-theta)/(2*PI);
        }
        else
        {
            u = 1-(theta/(2*PI));
        }



        v = 1 - (intersection.y + 0.5f);
    }


    toReturn.U = u;
    toReturn.V = v;



    return toReturn;
}


float Cylinder_Implicit::Intersect(const Ray &ray)
{

    glm::vec3 P = ray.orig;
    glm::vec3 d = ray.dir;



    if (d.y > 0 && P.y < -0.5) {
        float t1 = (-0.5 - P.y) / d.y;
        float rayX1 = P.x + d.x * t1;
        float rayZ1 = P.z + d.z * t1;
        if ((rayX1 * rayX1) + (rayZ1 * rayZ1) < 0.25) {
            return t1;
        }
    }


    if (d.y < 0 && P.y > 0.5) {
        float t2 = (0.5 - P.y) / d.y;
        float rayX2 = P.x + d.x * t2;
        float rayZ2 = P.z + d.z * t2;
        if ((rayX2 * rayX2) + (rayZ2 * rayZ2) < 0.25) {
            return t2;
        }
    }


    float a = d.x * d.x + d.z * d.z;
    float b = 2.0 * P.x * d.x + 2.0 * P.z * d.z;
    float c = P.x * P.x + P.z * P.z - 0.25;
    bool intersect = ImplicitShape::checkDiscriminant(a, b, c);
    if (intersect == false) {
        return -1;
    }
    float t = ImplicitShape::quadraticFormula(a, b, c);

    float rayY = P.y + d.y * t;
    if (rayY >= -0.5 && rayY <= 0.5) {
        return t;
    }

    return -1;
}





glm::vec3 Cylinder_Implicit::intersectNormal(glm::vec3 intersection)
{


    if (abs(intersection.y + 0.5) <= EPSILON) {
        return glm::vec3(0, -1, 0);
    }

    if (abs(intersection.y - 0.5) <= EPSILON) {
        return glm::vec3(0, 1, 0);
    }

    return glm::normalize(glm::vec3(intersection.x, 0, intersection.z));
}




