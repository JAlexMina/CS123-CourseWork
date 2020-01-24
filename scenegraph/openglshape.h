#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H
#define PI 3.14159265f

#include "GL/glew.h"

#include <memory>
#include <vector>

#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"

namespace CS123 { namespace GL {

class VAO;

}}

using namespace CS123::GL;

class OpenGLShape {
public:
    OpenGLShape();

    ~OpenGLShape();

    /** Initialize the VBO with the given vertex data. */
    void setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices);

    /** Enables the specified attribute and calls glVertexAttribPointer with the given arguments. */
    void setAttribute(GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type,
                      bool normalize);

    /** Build the VAO given the specified vertex data and atrributes */
    void buildVAO();

    /** Draw the initialized geometry. */
    void draw();

    /** Make a square on the four points given and draws it*/
    void MakeSquare(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, std::vector<float> vert4, int dir, int tess);

    /** interleaves the normals and arranges the verticies*/
    std::vector<GLfloat> interleave(std::vector<float> vertexData[], int tess, std::vector<float> normal);

    /** Make a triangle on the three points given and draws it*/
    void MakeTriangle(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, int dir, int tess, bool mp);

    void MakeCircle(int m_param2Tess, float m_radius, std::vector<float> m_vec1, int dir, int m_param1Tess);

    void MakePointy(int m_param2Tess, float m_radius, std::vector<float> m_vec1, int m_param1Tess);

    void MakeRectangle(std::vector<float> vert1, std::vector<float> vert2, std::vector<float> vert3, std::vector<float> vert4, int dir, int tess, std::vector<float> normal1, std::vector<float> normal2);

    void MakeSpheree(std::vector<float>m_center, float m_radius, float m_param1Tess, float m_param2Tess);

private:
    GLfloat *m_data;                            /// vector of floats containing the vertex data.
    GLsizeiptr m_size;                          /// size of the data array, in bytes.
    VBO::GEOMETRY_LAYOUT m_drawMode;            /// drawing mode
    int m_numVertices;                          /// number of vertices to be rendered
    std::vector<VBOAttribMarker> m_markers;     /// list of VBOAttribMarkers that describe how the data is laid out.
    std::unique_ptr<CS123::GL::VAO> m_VAO;      /// a wrapper for the vertex array object (VAO)
    std::unique_ptr<OpenGLShape> m_triangle;




};

#endif // OPENGLSHAPE_H
