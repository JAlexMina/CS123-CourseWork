#include "Cube.h"

Cube::Cube(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, std::vector<float> vert4,
           std::vector<float> vert5, std::vector<float> vert6, std::vector<float> vert7, std::vector<float> vert8, int tess) :
    m_vec1(vert1),
    m_vec2(vert2),
    m_vec3(vert3),
    m_vec4(vert4),
    m_vec5(vert5),
    m_vec6(vert6),
    m_vec7(vert7),
    m_vec8(vert8),
    m_tess(tess)
{
}


void Cube::MakeCube() {

    m_square->MakeSquare(m_vec1, m_vec2, m_vec3, m_vec4, 1, m_tess);
    m_square->MakeSquare(m_vec5, m_vec6, m_vec4, m_vec3, 1, m_tess);
    m_square->MakeSquare(m_vec6, m_vec5, m_vec8, m_vec7, 1, m_tess);
    m_square->MakeSquare(m_vec2, m_vec1, m_vec7, m_vec8, 1, m_tess);
    m_square->MakeSquare(m_vec2, m_vec7, m_vec4, m_vec5, 1, m_tess);
    m_square->MakeSquare(m_vec3, m_vec6, m_vec1, m_vec8, 1, m_tess);

}
