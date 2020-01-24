#include "Sphere.h"

Sphere::Sphere(std::vector<float> theCenter, float radius, int param2Tess, int param1Tess) :
    m_center(theCenter),
    m_radius(radius),
    m_param2Tess(param2Tess),
    m_param1Tess(param1Tess)
{

}


void Sphere::MakeSphere()
{
    MakeSpheree(m_center, m_radius, m_param1Tess, m_param2Tess);


}
