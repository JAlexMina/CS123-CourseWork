#ifndef CYLINDER_H
#define CYLINDER_H

#include "openglshape.h"

class Cylinder : OpenGLShape
{
public:
    Cylinder(float radius, float height, std::vector<float> center, int param2Tess, int param1Tess);

    void MakeCylinder();

private:

    std::vector<float> m_center;
    float m_radius;
    float m_height;
    int m_param2Tess;
    int m_param1Tess;
    std::unique_ptr<OpenGLShape> m_square;

};

#endif // CYLINDER_H
