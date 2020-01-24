#include "Cylinder.h"
#include <math.h>
#include <iostream>

Cylinder::Cylinder(float radius, float height, std::vector<float> center, int param2Tess, int param1Tess) :
    m_center(center),
    m_radius(radius),
    m_height(height),
    m_param2Tess(param2Tess),
    m_param1Tess(param1Tess)
{

}


void Cylinder::MakeCylinder()
{
    MakeCircle(m_param2Tess, m_radius, {m_center[0], m_center[1]+m_radius, m_center[2]}, 1, m_param1Tess);

    MakeCircle(m_param2Tess, m_radius, {m_center[0], m_center[1]-m_radius, m_center[2]}, -1, m_param1Tess);



    if(m_param2Tess < 3)
    {
        m_param2Tess = 3;
    }

    float q = 360.f/m_param2Tess;



    std::vector<float> normal1 = {0.f, 0.f, 0.f};
    std::vector<float> normal2 = {0.f, 0.f, 0.f};

    int j = 1;
    float theta = 0;
    float avt = 0;
    float x = m_radius * cos(theta*PI/180);
    float y = m_center[1]-m_radius;
    float z = m_radius * sin(theta*PI/180);

    for (int i = 0; i < m_param2Tess; i++) {

        float x2 = x;
        float y2 = y;
        float z2 = z;

        j++;

        normal2 = {cos(theta*PI/180.f), 0.f, sin(theta*PI/180.f)};
        theta = j * q /2;
        normal1 = {cos(theta*PI/180.f), 0.f, sin(theta*PI/180.f)};



        x = m_radius * cos(theta*PI/180);
        y = m_center[1]-m_radius;
        z = m_radius * sin(theta*PI/180);
        j++;



        m_square->MakeRectangle( {x2,y2,z2},  {x2,-1*y2,z2}, {x,y,z}, {x,-1*y,z}, -1 , m_param1Tess, normal1, normal2);



    }






}
