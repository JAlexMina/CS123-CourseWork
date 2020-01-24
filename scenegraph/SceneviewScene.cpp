#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"
#include <iostream>
#include "Settings.h"
#include "SupportCanvas3D.h"
#include <list>
#include <glm/gtx/string_cast.hpp>

#include "Cube.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"


using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();



}





SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}








void SceneviewScene::render(SupportCanvas3D *context) {
    //setClearColor();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //


    for (std::list<CS123SceneLightData>::iterator it = m_lightList.begin(); it != m_lightList.end(); ++it)
    {
        m_phongShader->setLight(*it);
    }


}




void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //




    //For Loop that goes through all
    //the items in the m_list
    for (std::list<item>::iterator it = m_list.begin(); it != m_list.end(); ++it)
    {



        //matrix - variable to hold the current transformations
        glm::mat4 matrix = it->transformations;
        string o = glm::to_string(matrix);

        //setting the mode view uniform
        m_phongShader->setUniform("m" , matrix);

        CS123SceneMaterial material;
        material.cDiffuse = it->scenePrimitive.material.cDiffuse * m_global.kd;
        material.cAmbient = it->scenePrimitive.material.cAmbient * m_global.ka;

        m_phongShader->applyMaterial(material);




        //switch to draw the correct primative
        switch (it->scenePrimitive.type)
        {

            case PrimitiveType::PRIMITIVE_CONE:
            {
                //making the cone
                float radius = 0.5;
                float height = 1;
                std::vector<float> theTip = {0.0f,  0.5f, 0.0f};
                m_cone = std::unique_ptr<Cone> (new Cone(radius, height, theTip, settings.shapeParameter2, settings.shapeParameter1));
                m_cone->MakeCone();
                break;
            }

            case PrimitiveType::PRIMITIVE_CUBE:
            {
                //making the cube
                std::vector<float> vert1 = {-0.5f, -0.5f, -0.5f, 1.0};
                std::vector<float> vert2 = {-0.5f, +0.5f, -0.5f, 1.0};
                std::vector<float> vert3 = {+0.5f, -0.5f, -0.5f, 1.0};
                std::vector<float> vert4 = {+0.5f, +0.5f, -0.5f, 1.0};

                std::vector<float> vert8 = {-0.5f, -0.5f, +0.5f, 1.0};
                std::vector<float> vert7 = {-0.5f, +0.5f, +0.5f, 1.0};
                std::vector<float> vert6 = {+0.5f, -0.5f, +0.5f, 1.0};
                std::vector<float> vert5 = {+0.5f, +0.5f, +0.5f, 1.0};
                m_cube = std::unique_ptr<Cube> (new Cube(vert1, vert2, vert3, vert4, vert5, vert6, vert7, vert8, settings.shapeParameter1));
                m_cube->MakeCube();
                break;
            }


            case PrimitiveType::PRIMITIVE_SPHERE:
            {
                //making the sphere
                std::vector<float> theCenter = {0.f, 0.f, 0.f};
                float radiusS = 0.5;
                m_sphere = std::unique_ptr<Sphere> (new Sphere(theCenter, radiusS, settings.shapeParameter2, settings.shapeParameter1));
                m_sphere->MakeSphere();
                break;
            }


            case PrimitiveType::PRIMITIVE_CYLINDER:
            {
                //making the cylinder
                std::vector<float> theCenterC = {0.f, 0.f, 0.f};
                float radiusC = 0.5;
                m_cylinder = std::unique_ptr<Cylinder> (new Cylinder(radiusC, 1, theCenterC, settings.shapeParameter2, settings.shapeParameter1));
                m_cylinder->MakeCylinder();
                break;
            }

        }


    }






}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
}

