#include "Cube_Implicit.h"
#include <math.h>
#include <bits/stdc++.h>


Cube_Implicit::Cube_Implicit()
{

}



UV Cube_Implicit::getUV(glm::vec4 intersection, int width, int height)
{
    UV toReturn;

    float u = 0;
    float v = 0;



    //first face
    if(std::abs(intersection.x-0.5f) <= EPSILON)
    {
       /* use y = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = 1-(intersection.z + 0.5f);
        v = 1-(intersection.y + 0.5f);

    }

    //second face
    if(std::abs(intersection.y-0.5f) <= EPSILON)
    {
       /* use x = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = (intersection.x + 0.5f);
        v = (intersection.z + 0.5f);

    }

    //third face
    if(std::abs(intersection.z-0.5f) <= EPSILON)
    {
       /* use x = [-0.5 -> 0.5]
          &   y = [-0.5 -> 0.5] */
        u = (intersection.x + 0.5f);
        v = 1-(intersection.y + 0.5f);

    }

    //////////////////////////////////////////////////////

    //fouth face AOK
    if(std::abs(intersection.x-(-0.5f)) <= EPSILON)
    {
       /* use y = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = (intersection.z + 0.5f);
        v = 1-(intersection.y + 0.5f);

    }

    //fifth face AOK
    if(std::abs(intersection.y-(-0.5f)) <= EPSILON)
    {
       /* use x = [-0.5 -> 0.5]
          &   z = [-0.5 -> 0.5] */
        u = (intersection.x + 0.5f);
        v = 1-(intersection.z + 0.5f);

    }

    //sixth face AOK
    if(std::abs(intersection.z-(-0.5f)) <= EPSILON)
    {
       /* use x = [-0.5 -> 0.5]
          &   y = [-0.5 -> 0.5] */
        u = 1-(intersection.x + 0.5f);
        v = 1-(intersection.y + 0.5f);

    }



    toReturn.U = u;
    toReturn.V = v;




    return toReturn;
}











float Cube_Implicit::Intersect(const Ray &ray)
{

    glm::vec3 P = ray.orig;
    glm::vec3 d = ray.dir;


    float t1;
    float t2;
    float temp;
    float tCube;
    float tnear = -std::numeric_limits<float>::infinity();
    float tfar = std::numeric_limits<float>::infinity();
    glm::vec3 min = glm::vec3(-0.5, -0.5, -0.5);
    glm::vec3 max = glm::vec3(0.5, 0.5, 0.5);
    bool checkIntersect = true;


    if (abs(d.x-0) < EPSILON) {
        if (P.x < min.x || P.x > max.x) {
            checkIntersect = false;
        }
    }
    else {
        t1 = (min.x - P.x) / d.x;
        t2 = (max.x - P.x) / d.x;
        if (t1 > t2) {
            temp = t1;
            t1 = t2;
            t2 = temp;
        }
        if (t1 > tnear) {
            tnear = t1;
        }
        if (t2 < tfar) {
            tfar = t2;
        }
        if (tnear > tfar) {
            checkIntersect = false;
        }
        if (tfar < 0) {
            checkIntersect = false;
        }
    }

    if (abs(d.y-0) < EPSILON) {
        if (P.y < min.y || P.y > max.y) {
            checkIntersect = false;
        }
    }
    else {
        t1 = (min.y - P.y) / d.y;
        t2 = (max.y - P.y) / d.y;
        if (t1 > t2) {
            temp = t1;
            t1 = t2;
            t2 = temp;
        }
        if (t1 > tnear) {
            tnear = t1;
        }
        if (t2 < tfar) {
            tfar = t2;
        }
        if (tnear > tfar) {
            checkIntersect = false;
        }
        if (tfar < 0) {
            checkIntersect = false;
        }
    }

    if (d.z == 0) {
        if (P.z < min.z || P.z > max.z) {
            checkIntersect = false;
        }
    }
    else {
        t1 = (min.z - P.z) / d.z;
        t2 = (max.z - P.z) / d.z;
        if (t1 > t2) {
            temp = t1;
            t1 = t2;
            t2 = temp;
        }
        if (t1 > tnear) {
            tnear = t1;
        }
        if (t2 < tfar) {
            tfar = t2;
        }
        if (tnear > tfar) {
            checkIntersect = false;
        }
        if (tfar < 0) {
            checkIntersect = false;
        }
    }
    if (checkIntersect == false) {
        tCube = -1;
    }
    else {
        tCube = tnear;
    }
    return tCube;
}





glm::vec3 Cube_Implicit::intersectNormal(glm::vec3 intersection)
{


    if (abs(intersection.x - (-0.5f)) <= EPSILON) {
        return glm::vec3(-1, 0, 0);
    }
    if (abs(intersection.x - (0.5f)) <= EPSILON) {
        return glm::vec3(1, 0, 0);
    }
    if (abs(intersection.y - (-0.5f)) <= EPSILON) {
        return glm::vec3(0, -1, 0);
    }
    if (abs(intersection.y - (0.5f)) <= EPSILON) {
        return glm::vec3(0, 1, 0);
    }
    if (abs(intersection.z - (-0.5f)) <= EPSILON) {
        return glm::vec3(0, 0, -1);
    }
    if (abs(intersection.z - (0.5f)) <= EPSILON) {
        return glm::vec3(0, 0, 1);
    }
}


