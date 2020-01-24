#include "Cone.h"
#include <math.h>
#include <iostream>
#define PI 3.14159265

Cone::Cone(float radius, float height, std::vector<float> theTip, int param2Tess, int param1Tess) :
    m_vec1(theTip),
    m_radius(radius),
    m_height(height),
    m_param2Tess(param2Tess),
    m_param1Tess(param1Tess)
{

}






void Cone::MakeCone()
{


    MakePointy(m_param2Tess, m_radius, m_vec1, m_param1Tess);

    MakeCircle(m_param2Tess, m_radius, {-1*m_vec1[0], -1*m_vec1[1], -1*m_vec1[2]}, -1, m_param1Tess);




}


