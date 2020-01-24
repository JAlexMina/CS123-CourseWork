#include "openglshape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/datatype/VAO.h"
#include <numeric>
#include <iostream>
#include <vector>
#include <math.h>
#include <functional>
#include <assert.h>
#include "glm.hpp"

using namespace CS123::GL;

OpenGLShape::OpenGLShape() :
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_numVertices(0),
    m_VAO(nullptr)
{
    // You shouldn't need to do anything here
    // Aren't initializer lists amazing?
}

OpenGLShape::~OpenGLShape()
{
    // You shouldn't need to do anything here
    // m_VAO's destructor will automatically be called
    // Aren't smart pointers amazing?
}


/**
 * @param data - Vector of floats containing the vertex data.
 * @param size - number of elements in the array.
 * @param drawMode - Drawing mode
 * @param numVertices - Number of vertices to be rendered.
 */
void OpenGLShape::setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices) {
    // Store the vertex data and other values to be used later when constructing the VAO
    m_data = data;
    m_size = size;
    m_drawMode = drawMode;
    m_numVertices = numVertices;
}


/**
 * @param index OpenGL handle to the attribute location. These are specified in ShaderAttribLocations.h
 * @param numElementsPerVertex Number of elements per vertex. Must be 1, 2, 3 or 4 (e.g. position = 3 for x,y,z)
 * @param offset Offset in BYTES from the start of the array to the beginning of the first element
 * @param type Primitive type (FLOAT, INT, UNSIGNED_BYTE)
 * @param normalize
 */
void OpenGLShape::setAttribute(
         GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type, bool normalize) {
    m_markers.push_back(VBOAttribMarker(index, numElementsPerVertex, offset, type, normalize));
}

void OpenGLShape::buildVAO() {
    CS123::GL::VBO vbo = VBO(m_data, m_size, m_markers, m_drawMode);
    m_VAO = std::make_unique<VAO>(vbo, m_numVertices);
}

void OpenGLShape::draw() {
    if (m_VAO) {
        // TODO [Task 5]
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}


//function to add three vectors together
std::vector<float> add2Vectors(std::vector<float> v1, std::vector<float> v2)
{
    return {(v1[0]+v2[0]), (v1[1]+v2[1]), (v1[2]+v2[2])};
}


// Function to find
// cross product of two vector array.
void crossProduct(std::vector<float> vect_A, std::vector<float> vect_B, std::vector<float> *cross_P, float dir)
{



    //glm::cross(vect_A, vect_B);

    (*cross_P)[0] = ((vect_A[1] * vect_B[2]) - (vect_A[2] * vect_B[1])) * dir;
    (*cross_P)[1] = ((vect_B[0] * vect_A[2]) - (vect_B[2] * vect_A[0])) * dir;
    (*cross_P)[2] = ((vect_A[0] * vect_B[1]) - (vect_A[1] * vect_B[0])) * dir;



    //normalizing
    float l = sqrt(((*cross_P)[0]*(*cross_P)[0]) + ((*cross_P)[1]*(*cross_P)[1]) + ((*cross_P)[2]*(*cross_P)[2]));

    (*cross_P)[0] = (*cross_P)[0] / l;
    (*cross_P)[1] = (*cross_P)[1] / l;
    (*cross_P)[2] = (*cross_P)[2] / l;
}


//function to add three vectors together
std::vector<float> add3Vectors(std::vector<float> v1, std::vector<float> v2, std::vector<float> v3)
{
    return {(v1[0]+v2[0]+v3[0]), (v1[1]+v2[1]+v3[1]), (v1[2]+v2[2]+v3[2])};
}

//function used to find i and j
std::vector<float> sub2Vectors(std::vector<float> v1, std::vector<float> v2, int tess)
{
    return {(v1[0]-v2[0])/tess, (v1[1]-v2[1])/tess, (v1[2]-v2[2])/tess};
}

//function to multiply a vector by a scalar
std::vector<float> multvect(std::vector<float> v1, float i) {
    return {v1[0]*i, v1[1]*i, v1[2]*i};
}

//function to get the index of the 2d representation in the 1d array
int get_index(int row, int col, int num_cols) {

  return ((row * num_cols) + col);
}


void OpenGLShape::MakeRectangle(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, std::vector<float> vert4, int dir, int tess, std::vector<float> normal1, std::vector<float> normal2)
{
    //std::vector<float> normal = {cos(theta*PI/180.f), 0.f, sin(theta*PI/180.f)};
//    std::vector<float> one = {vert1[0]-vert2[0], vert1[1]-vert2[1], vert1[2]-vert2[2]};
//    std::vector<float> two = {vert4[0]-vert2[0], vert4[1]-vert2[1], vert4[2]-vert2[2]};

//    crossProduct(one, two, &normal, dir);

    std::vector<float> i = {0.f, 0.f, 0.f};
    std::vector<float> j = {0.f, 0.f, 0.f};
    std::vector<float> zero = {0.f, 0.f, 0.f};



    std::vector<GLfloat> rectData;;
    rectData.resize((tess+1)*12);

    i = sub2Vectors(vert1, vert2, tess);
    j = sub2Vectors(vert4, vert2, 100000);

    int cnt = 0;




    for(int row = 0; row < (tess+1); row++, cnt += 12)
    {

            std::vector<float> v2ANDi = add3Vectors(vert2, multvect(i, row), zero);
            rectData[cnt]   = v2ANDi[0];
            rectData[cnt+1] = v2ANDi[1];
            rectData[cnt+2] = v2ANDi[2];
            rectData[cnt+3] = normal2[0];
            rectData[cnt+4] = normal2[1];
            rectData[cnt+5] = normal2[2];



            std::vector<float> v4ANDj = add3Vectors(vert4, multvect(i, row), j);
            rectData[cnt+6]   = v4ANDj[0];
            rectData[cnt+7] = v4ANDj[1];
            rectData[cnt+8] = v4ANDj[2];
            rectData[cnt+9] = normal1[0];
            rectData[cnt+10] = normal1[1];
            rectData[cnt+11] = normal1[2];









    }



    int kFloatsPerVertex = 6;

    std::unique_ptr<OpenGLShape> s = std::make_unique<OpenGLShape>();
    s->setVertexData(rectData.data(), rectData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, rectData.size() / kFloatsPerVertex);
    s->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    s->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    s->buildVAO();


    s->draw();
}













void OpenGLShape::MakeSpheree(std::vector<float>m_center, float m_radius, float m_param1Tess, float m_param2Tess)
{
    float dir = 1;

    if(m_param2Tess < 3)
    {
        m_param2Tess = 3;
    }



        float q = 360.f/m_param2Tess;

    //    std::cout << q << std::endl;


        int j = 1;
        float theta = 0;
        float x = m_radius * cos(theta*PI/180);
        float y = m_center[1];
        float z = m_radius * sin(theta*PI/180);

        for (int i = 0; i < m_param2Tess; i++) {

            float x2 = x;
            float y2 = y;
            float z2 = z;

            j++;

            theta = j * q /2;


            x = m_radius * cos(theta*PI/180);
            y = m_center[1];
            z = m_radius * sin(theta*PI/180);
            j++;



                m_triangle->MakeTriangle( {x,y,z}, {x2,y2,z2}, {m_center[0], (m_center[1] + 0.25f + (1.25f/m_param1Tess)), m_center[2]}, -1 , 1, false);

                m_triangle->MakeTriangle( {m_center[0], (m_center[1] - 0.25f - (1.25f/m_param1Tess)), m_center[2]}, {x2,y2,z2}, {x,y,z}, -1 , 1, false);

        }



}













void OpenGLShape::MakePointy(int m_param2Tess, float m_radius, std::vector<float> m_vec1, int m_param1Tess)
{
    if(m_param2Tess < 3)
    {
        m_param2Tess = 3;
    }



        float q = 360.f/m_param2Tess;

        int j = 1;
        float theta = 0;
        float x = m_radius * cos(theta*PI/180);
        float y = m_vec1[1]-m_radius*2;
        float z = m_radius * sin(theta*PI/180);

        for (int i = 0; i < m_param2Tess; i++) {

            float x2 = x;
            float y2 = y;
            float z2 = z;

            j++;

            theta = j * q /2;


            x = m_radius * cos(theta*PI/180);
            y = m_vec1[1]-m_radius*2;
            z = m_radius * sin(theta*PI/180);
            j++;


            m_triangle->MakeTriangle({x2,y2,z2}, m_vec1, {x,y,z}, -1 , m_param1Tess, true);



        }
}


void OpenGLShape::MakeTriangle(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, int dir, int tess, bool mp)
{
//    std::unique_ptr<OpenGLShape> t = std::make_unique<OpenGLShape>();

    std::vector<float> normal = {0.f, 0.f, 0.f};
    float m = 0;





        std::vector<float> one   = {vert1[0]-vert2[0], vert1[1]-vert2[1], vert1[2]-vert2[2]};
        std::vector<float> two   = {vert3[0]-vert2[0], vert3[1]-vert2[1], vert3[2]-vert2[2]};


        crossProduct(one  , two , &normal , dir);






//TESSALATING THE FACE OF THE TRIANGLE////////////////////////////////////////////////////////////


        std::vector<float> i = {0.f, 0.f, 0.f};
        std::vector<float> j = {0.f, 0.f, 0.f};
        std::vector<float> zero = {0.f, 0.f, 0.f};



        std::vector<GLfloat> pos;;
        pos.resize((tess+1)*12);

        i = sub2Vectors(vert1, vert2, tess);
        j = sub2Vectors(vert3, vert2, tess);

        int cnt = 0;




        for(int row = 0; row < (tess+1); row++, cnt += 12)
        {





                std::vector<float> v2ANDi = add2Vectors(vert2, multvect(i, row));

                if(mp && row != 0)
                {

                    m = sqrt(pow(v2ANDi[0], 2) + pow(v2ANDi[2], 2));
                    normal = {v2ANDi[0]/m, (0.5f/1.f), v2ANDi[2]/m};
                }


                pos[cnt]   = v2ANDi[0];
                pos[cnt+1] = v2ANDi[1];
                pos[cnt+2] = v2ANDi[2];

                pos[cnt+3] = normal[0];//*(1.f/0.5f);
                pos[cnt+4] = normal[1];//;
                pos[cnt+5] = normal[2];//*(1.f/0.5f);



                std::vector<float> v4ANDj = add2Vectors(vert2, multvect(j, row));
                if(mp && row != 0)
                {
                    m = sqrt(pow(v4ANDj[0], 2) + pow(v4ANDj[2], 2));
                    normal = {v4ANDj[0]/m, (0.5f/1.f), v4ANDj[2]/m};
                }

                pos[cnt+6]   = v4ANDj[0];
                pos[cnt+7] = v4ANDj[1];
                pos[cnt+8] = v4ANDj[2];

                pos[cnt+9]  = normal[0];//*(1.f/0.5f);
                pos[cnt+10] = normal[1];
                pos[cnt+11] = normal[2];//*(1.f/0.5f);



        }





    int kFloatsPerVertex = 6;

    std::unique_ptr<OpenGLShape> s = std::make_unique<OpenGLShape>();
    s->setVertexData(pos.data(), pos.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, pos.size() / kFloatsPerVertex);
    s->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    s->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    s->buildVAO();


    s->draw();


}


void OpenGLShape::MakeCircle(int m_param2Tess, float m_radius, std::vector<float> m_vec1, int dir, int m_param1Tess)
{
    if(m_param2Tess < 3)
    {
        m_param2Tess = 3;
    }



        float q = 360.f/m_param2Tess;

    //    std::cout << q << std::endl;


        int j = 1;
        float theta = 0;
        float x = m_radius * cos(theta*PI/180);
        float y = m_vec1[1]*1;
        float z = m_radius * sin(theta*PI/180);

        for (int i = 0; i < m_param2Tess; i++) {

            float x2 = x;
            float y2 = y;
            float z2 = z;

            j++;

            theta = j * q /2;


            x = m_radius * cos(theta*PI/180);
            y = m_vec1[1]*1;
            z = m_radius * sin(theta*PI/180);
            j++;


            if(dir == -1)
            {
                m_triangle->MakeTriangle({x,y,z}, m_vec1, {x2,y2,z2},  -1 , m_param1Tess, false);
            }
            else
            {
                m_triangle->MakeTriangle({x2,y2,z2}, m_vec1, {x,y,z}, -1 , m_param1Tess, false);
            }
        }
}




void OpenGLShape::MakeSquare(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, std::vector<float> vert4, int dir, int tess){

        std::vector<float> normal = {0.f, 0.f, 0.f};
        std::vector<float> one = {vert1[0]-vert2[0], vert1[1]-vert2[1], vert1[2]-vert2[2]};
        std::vector<float> two = {vert4[0]-vert2[0], vert4[1]-vert2[1], vert4[2]-vert2[2]};

        crossProduct(one, two, &normal, -dir);

        std::vector<float> i = {0.f, 0.f, 0.f};
        std::vector<float> j = {0.f, 0.f, 0.f};

        std::vector<float> pos[(tess+1)*(tess+1)];

        i = sub2Vectors(vert4, vert2, tess);
        j = sub2Vectors(vert1, vert2, tess);



        for(int row = 0; row < (tess+1); row++)
        {
            for(int col = 0; col < (tess+1); col++)
            {

                pos[get_index(row, col, (tess+1))] = add3Vectors(vert2, multvect(i, row), multvect(j, col));

            }
        }


        std::vector<GLfloat> squareData = interleave(pos, tess, normal);
        int kFloatsPerVertex = 6;

        std::unique_ptr<OpenGLShape> s = std::make_unique<OpenGLShape>();
        s->setVertexData(squareData.data(), squareData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, squareData.size() / kFloatsPerVertex);
        s->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
        s->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
        s->buildVAO();


        s->draw();

}

std::vector<GLfloat> OpenGLShape::interleave(std::vector<float> vertexData[], int tess, std::vector<float> normal)
{

    std::vector<GLfloat> squareData = {};

    squareData.resize((tess+1)*3*(tess+1)*3*2);

    //Interleaving the normal data
    int i = 0;
    for(int row = 0; row < (tess); row++)
    {
        squareData[i]   = vertexData[get_index(row, 0, (tess+1))][0];
        squareData[i+1] = vertexData[get_index(row, 0, (tess+1))][1];
        squareData[i+2] = vertexData[get_index(row, 0, (tess+1))][2];
        squareData[i+3] = normal[0];
        squareData[i+4] = normal[1];
        squareData[i+5] = normal[2];

        i = i + 6;

        squareData[i]   = vertexData[get_index(row+1, 0, (tess+1))][0];
        squareData[i+1] = vertexData[get_index(row+1, 0, (tess+1))][1];
        squareData[i+2] = vertexData[get_index(row+1, 0, (tess+1))][2];
        squareData[i+3] = normal[0];
        squareData[i+4] = normal[1];
        squareData[i+5] = normal[2];

        i = i + 6;

        for(int col = 0; col < (tess); col++)
        {
            squareData[i]   = vertexData[get_index(row, col+1, (tess+1))][0];
            squareData[i+1] = vertexData[get_index(row, col+1, (tess+1))][1];
            squareData[i+2] = vertexData[get_index(row, col+1, (tess+1))][2];
            squareData[i+3] = normal[0];
            squareData[i+4] = normal[1];
            squareData[i+5] = normal[2];

            i = i + 6;

            squareData[i]   = vertexData[get_index(row+1, col+1, (tess+1))][0];
            squareData[i+1] = vertexData[get_index(row+1, col+1, (tess+1))][1];
            squareData[i+2] = vertexData[get_index(row+1, col+1, (tess+1))][2];
            squareData[i+3] = normal[0];
            squareData[i+4] = normal[1];
            squareData[i+5] = normal[2];

            i = i + 6;
        }

    }

    return squareData;


}
