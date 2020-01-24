#ifndef CUBE_H
#define CUBE_H

#include "openglshape.h"


class Cube : OpenGLShape
{
    public:
        //constructor
        Cube(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, std::vector<float> vert4,
             std::vector<float> vert5, std::vector<float> vert6, std::vector<float> vert7, std::vector<float> vert8, int tess);

        void MakeCube();

    private:
        std::unique_ptr<OpenGLShape> m_square;
//        std::unique_ptr<std::vector<float>> m_vec1;
//        std::unique_ptr<std::vector<float>> m_vec2;
//        std::unique_ptr<std::vector<float>> m_vec3;
//        std::unique_ptr<std::vector<float>> m_vec4;

        std::vector<float> m_vec1;
        std::vector<float> m_vec2;
        std::vector<float> m_vec3;
        std::vector<float> m_vec4;
        std::vector<float> m_vec5;
        std::vector<float> m_vec6;
        std::vector<float> m_vec7;
        std::vector<float> m_vec8;
        int m_tess;

};

#endif // CUBE_H
