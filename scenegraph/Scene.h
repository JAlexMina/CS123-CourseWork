#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Cube.h"
#include <QImage>

#include "Cone_Implicit.h"
#include "Cylinder_Implicit.h"
#include "Sphere_Implicit.h"
#include "Cube_Implicit.h"



class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();


    std::list<item> m_list;

    std::list<item> m_listCHECK;
    std::list<CS123SceneLightData> m_lightList;
    CS123SceneGlobalData m_global;


    std::unique_ptr<Cone> m_cone;
    std::unique_ptr<Cube> m_cube;
    std::unique_ptr<Sphere> m_sphere;
    std::unique_ptr<Cylinder> m_cylinder;


    std::unique_ptr<Cone_Implicit> m_coneI;
    std::unique_ptr<Cube_Implicit> m_cubeI;
    std::unique_ptr<Sphere_Implicit> m_sphereI;
    std::unique_ptr<Cylinder_Implicit> m_cylinderI;






    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);



protected:

    // Adds a primitive to the scene.


    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);





};

#endif // SCENE_H
