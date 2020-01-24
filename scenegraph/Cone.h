#ifndef CONE_H
#define CONE_H

#include "openglshape.h"

class Cone : OpenGLShape
{
public:
    Cone(float radius, float height, std::vector<float> theTip, int param2Tess, int param1Tess);

    void MakeCone();

private:
    std::unique_ptr<OpenGLShape> m_triangle;

    std::vector<float> m_vec1;
    float m_radius;
    float m_height;
    float m_param2Tess;
    float m_param1Tess;


};

#endif // CONE_H
