#ifndef IMPLICITSHAPE_H
#define IMPLICITSHAPE_H

#define EPSILON 0.00001f
#define PI 3.14159265359f

#include "glm/glm.hpp"

struct Ray
{
   glm::vec3 orig;
   glm::vec3 dir;
};

struct UV
{
   float U;
   float V;
};



class ImplicitShape
{
public:
    ImplicitShape();
    virtual ~ImplicitShape();

    //virtual TS getTS(glm::vec4 intersection);
    static bool checkDiscriminant(float a, float b, float c);
    static float quadraticFormula(float a, float b, float c);
};

#endif // IMPLICITSHAPE_H
