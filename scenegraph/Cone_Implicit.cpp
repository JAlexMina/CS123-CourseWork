#include "Cone_Implicit.h"



Cone_Implicit::Cone_Implicit()
{

}


UV Cone_Implicit::getUV(glm::vec4 intersection)
{
    UV toReturn;
    float u;
    float v;


    //first face
    if(std::abs(intersection.y+0.5f) <= EPSILON)
    {
       /* use y = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = (intersection.x + 0.5f);
        v = 1-(intersection.z + 0.5f);

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



float Cone_Implicit::Intersect(const Ray &ray)
{

    glm::vec3 P = ray.orig;
    glm::vec3 d = ray.dir;



        if (d.y > 0.0 && P.y < -0.5) {
            float t1 = -(0.5 + P.y) / d.y;
            float rayX = P.x + d.x * t1;
            float rayZ = P.z + d.z * t1;
            if ((rayX * rayX) + (rayZ * rayZ) < 0.25) {
                return t1;
            }
        }


        float a = (d.x * d.x) + (d.z * d.z) - (d.y * d.y * 0.25);
        float b = (2.0 * P.x * d.x) + (2.0 * P.z * d.z) + (d.y * 0.25) - (P.y * d.y * 0.5);
        float c = (P.x * P.x) + (P.z * P.z) + (P.y * 0.25) - (P.y * P.y * 0.25) - 0.0625;

        if (ImplicitShape::checkDiscriminant(a, b, c) == false) {
            return -1;
        }

        float t = ImplicitShape::quadraticFormula(a, b, c);

        float rayY = P.y + d.y * t;
        if (rayY >= -0.5 && rayY <= 0.5) {
            return t;
        }

        return -1;

 }






glm::vec3 Cone_Implicit::intersectNormal(glm::vec3 intersection)
{


    if (std::abs(intersection.y + 0.5) <= EPSILON) {
        return glm::vec3(0, -1, 0);
    }
    return glm::normalize(glm::vec3(2.0 * intersection.x, sqrt(intersection.x * intersection.x + intersection.z * intersection.z), 2.0 * intersection.z));
}
