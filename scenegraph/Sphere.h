#ifndef SPHERE_H
#define SPHERE_H

#include "openglshape.h"

class Sphere : OpenGLShape
{
public:
    Sphere(std::vector<float> theCenter, float radius, int param2Tess, int param1Tess);
    void MakeSphere();

private:


    std::vector<float> m_center;
    std::unique_ptr<OpenGLShape> m_sphere;
    float m_radius;
    int m_param2Tess;
    int m_param1Tess;
};

#endif // SPHERE_H
