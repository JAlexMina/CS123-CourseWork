#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"
#include <glm/gtc/matrix_transform.hpp>
#include <bits/stdc++.h>
#include <glm/gtx/string_cast.hpp>
#include <string>


#include "Settings.h"
#include "SupportCanvas3D.h"
#include "Cube.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <iostream>
#include "glm/gtx/transform.hpp"
#include "CS123SceneData.h"
//#include "SceneviewScene.h"



Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    m_list = scene.m_list;
    m_lightList = scene.m_lightList;
    m_global = scene.m_global;

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}







//recursive funtion to go through the scene graph
void recFunc(std::list<item> *lst, CS123SceneNode *node, glm::mat4 matrix, Scene *sceneToFill)
{


    //Going through the transformations
    //building the composite matrix
    for (size_t i = 0; i < node->transformations.size(); i++)
    {

        switch (node->transformations[i]->type)
        {
            case TRANSFORMATION_SCALE:
            {
                glm::mat4 myScaleMatrix = glm::scale(glm::mat4(), node->transformations[i]->scale);
                matrix = matrix * myScaleMatrix;
                break;
            }

            case TRANSFORMATION_ROTATE:
            {
                glm::mat4 myRotMatrix = glm::rotate(node->transformations[i]->angle, node->transformations[i]->rotate);
                matrix = matrix * myRotMatrix;
                break;
            }

            case TRANSFORMATION_TRANSLATE:
            {
                glm::mat4 myTransMatrix = glm::translate(glm::mat4(), node->transformations[i]->translate);
                matrix = matrix * myTransMatrix;
                break;
            }

            case TRANSFORMATION_MATRIX:
            {
                matrix = matrix * node->transformations[i]->matrix;
                break;
            }
        }
    }


    //Adding each primative in this node
    //to the sceneToFill's m_list
    for (size_t i = 0; i < node->primitives.size(); i++)
    {
        sceneToFill->addPrimitive(*node->primitives[i], matrix);
    }


    //Calling this function recursively
    //for all of this node's children
    for(size_t j = 0; j < node->children.size(); j++)
    {
        recFunc(lst, node->children[j], matrix, sceneToFill);
    }

}





void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()


    CS123SceneLightData light;

    for (int i = 0; i < parser->getNumLights(); i++)
    {
        parser->getLightData(i, light);
        sceneToFill->addLight(light);
    }


    CS123SceneGlobalData global;
    parser->getGlobalData(global);
    sceneToFill->setGlobal(global);



    CS123SceneNode *node = parser->getRootNode();
    glm::mat4 myIdentityMatrix = glm::mat4(1.0f);

    recFunc(&sceneToFill->m_list, node, myIdentityMatrix, sceneToFill);



}


//method to add a primative and its
//corresponding matrix to the caller's m_list
void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {

    item current;

    current.transformations = matrix;
    CS123ScenePrimitive transfer = scenePrimitive;
    current.scenePrimitive  = transfer;
    this->m_list.push_back(current);

}


void Scene::addLight(const CS123SceneLightData &sceneLight) {

    CS123SceneLightData transfer = sceneLight;
    this->m_lightList.push_back(transfer);

}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    CS123SceneGlobalData transfer = global;
    this->m_global = transfer;
}

