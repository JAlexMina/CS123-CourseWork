#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "Canvas2D.h"

#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();

    void renderScene(Canvas2D *canvas, Camera *camera);
    Scene m_oldScene;

    int get_index(int row, int col, int num_cols);

    glm::vec3 cameraToWorldSpace(glm::mat4x4 cameraViewMatrix, glm::mat4x4 cameraScaleMatrix, glm::vec3 cameraSpacePoint);
    glm::vec3 screenToCameraSpace(int x, int y, int xMax, int yMax);
    glm::vec3 worldToObjectSpace(glm::mat4x4 compositeMatrix, glm::vec3 worldSpacePoint);
    glm::vec3 worldToObjectSpaceDir(glm::mat4x4 compositeMatrix, glm::vec3 worldSpacePoint);
    glm::vec3 normalVectorToWorld(glm::mat4x4 matrix, glm::vec3 normalVectorInObject);


    RGBA illuminate(float shortestT, struct Ray localRay, glm::vec3 norm, Canvas2D *canvas, CS123ScenePrimitive prim, int x, int y, struct Ray ray,
                    glm::vec3 directionInWorld, glm::vec3 eyeInWorld, glm::vec4 intersectionPos, QImage textureList[], int is);




    RGBA rend(struct Ray ray, struct Ray localRay, Canvas2D *canvas, int x, int y, glm::vec3 directionInWorld, glm::vec3 eyeInWorld, QImage textureList[]);

    float m_width;
    float m_height;
    int m_recursionLimit;

};



#endif // RAYSCENE_H
