#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include "Sphere_Implicit.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <QImage>
#include <QString>
#include <QRgb>
#include <iostream>


RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
    m_list = scene.m_list;
    m_lightList = scene.m_lightList;
    m_global = scene.m_global;

}


int RayScene::get_index(int row, int col, int num_cols) {

  return ((row * num_cols) + col);
}



glm::vec3 RayScene::cameraToWorldSpace(glm::mat4x4 cameraViewMatrix, glm::mat4x4 cameraScaleMatrix, glm::vec3 cameraSpacePoint)
{
    glm::mat4x4 inverseViewMatrix = glm::inverse(cameraViewMatrix);
    glm::mat4x4 inverseScalingMatrix = glm::inverse(cameraScaleMatrix);
    glm::vec4 worldVectorV4 = inverseViewMatrix * inverseScalingMatrix * glm::vec4(cameraSpacePoint, 1.0);
    glm::vec3 worldVector = glm::vec3(worldVectorV4[0], worldVectorV4[1], worldVectorV4[2]) / worldVectorV4[3];
    return worldVector;
}


glm::vec3 RayScene::screenToCameraSpace(int x, int y, int xMax, int yMax)
{
    float xCamSpace = (2.0 * x / xMax) - 1.0;
    float yCamSpace = 1.0 - (2.0 * y / yMax);
    float zCamSpace = -1.0;
    glm::vec3 camSpaceCoord = glm::vec3(xCamSpace, yCamSpace, zCamSpace);
    return camSpaceCoord;
}



glm::vec3 RayScene::worldToObjectSpace(glm::mat4x4 compositeMatrix, glm::vec3 worldSpacePoint)
{
    glm::mat4x4 inverseCompositeMatrix = glm::inverse(compositeMatrix);
    glm::vec4 objectVectorV4 = inverseCompositeMatrix * glm::vec4(worldSpacePoint, 1.0);
    glm::vec3 objectVector = glm::vec3(objectVectorV4[0], objectVectorV4[1], objectVectorV4[2]) / objectVectorV4[3];
    return objectVector;
}

glm::vec3 RayScene::worldToObjectSpaceDir(glm::mat4x4 compositeMatrix, glm::vec3 worldSpacePoint)
{
    glm::mat4x4 inverseCompositeMatrix = glm::inverse(compositeMatrix);
    glm::vec4 objectVectorV4 = inverseCompositeMatrix * glm::vec4(worldSpacePoint, 0.0);
    glm::vec3 objectVector = glm::vec3(objectVectorV4[0], objectVectorV4[1], objectVectorV4[2]);
    return objectVector;
}

glm::vec3 RayScene::normalVectorToWorld(glm::mat4x4 matrix, glm::vec3 normalVectorInObject)
{
    glm::mat3x3 M3 = glm::mat3x3(matrix);
    glm::mat3x3 inverseTransposeMatrix = glm::transpose(glm::inverse(M3));
    glm::vec3 normalVectorInWorld = glm::mat3(inverseTransposeMatrix) * normalVectorInObject;
    return normalVectorInWorld;
}


RGBA RayScene::illuminate(float shortestT, struct Ray localRay, glm::vec3 norm, Canvas2D *canvas, CS123ScenePrimitive prim, int x, int y, struct Ray ray, glm::vec3 directionInWorld, glm::vec3 eyeInWorld, glm::vec4 intersectionPos, QImage textureList[], int is)
{
    RGBA c = RGBA(0,0,0,0);


    //if the ray hit something
    if(shortestT != std::numeric_limits<float>::max())
    {


        glm::vec4 accumulater = glm::vec4(0, 0, 0, 0);
        glm::vec4 accumulaterCurrentLight = glm::vec4(0, 0, 0, 0);

        glm::vec4 locIntersect = glm::vec4(localRay.orig, 0.0);
        glm::vec3 dirShadowWorld;
        glm::vec3 L;
        glm::vec3 normal = localRay.dir;
        float att = 1;






    //for every light in the scene
    for(std::list<CS123SceneLightData>::iterator it = m_lightList.begin(); it != m_lightList.end(); ++it)
    {

         switch(it->type)
         {
            case LightType::LIGHT_POINT:
             {

                if (settings.useShadows)
                {
                    dirShadowWorld = glm::normalize(glm::vec3(it->pos[0], it->pos[1], it->pos[2]) - glm::vec3(intersectionPos));
                }
                L = glm::normalize(glm::vec3(it->pos) - glm::vec3(intersectionPos));
                att = glm::length(it->pos - intersectionPos);


              break;

             }

             case LightType::LIGHT_DIRECTIONAL:
             {
                if (settings.useShadows)
                {
                    dirShadowWorld = -glm::normalize(glm::vec3(it->dir[0], it->dir[1], it->dir[2]));
                }

                L = -glm::normalize(glm::vec3(it->dir));


                 break;

             }

             case LightType::LIGHT_SPOT:
             {
                 break;
             }

             case LightType::LIGHT_AREA:
             {
                 break;
             }
         }



         //////////////////////////////////////////////



         bool isInShadow = false;






             //check for shadows
             if (settings.useShadows)
             {
                 //glm::vec3 dirShadowWorld = -glm::normalize(glm::vec3(it->dir[0], it->dir[1], it->dir[2]));
                 glm::vec3 eyeShadowWorld = glm::vec3(intersectionPos);



                 //std::map<std::unique_ptr<Mesh>, PrimitiveAndMatrix>::iterator iter2;
                 //for (iter2 = m_meshMatrixMap.begin(); iter2 != m_meshMatrixMap.end(); iter2++)
                 for (std::list<item>::iterator iter2 = m_list.begin(); iter2 != m_list.end(); ++iter2)
                 {

                     glm::mat4x4 primitiveMatrixShadow = iter2->transformations;
                     glm::vec3 eyeShadowObject = worldToObjectSpace(primitiveMatrixShadow, eyeShadowWorld);
                     glm::vec3 dirShadowObject = worldToObjectSpaceDir(primitiveMatrixShadow, dirShadowWorld);

                     glm::vec3 offset = 0.001f * normal;

                     float tShadow = 1;

                     localRay.dir = dirShadowObject;
                     localRay.orig = eyeShadowObject + offset;



                     switch (iter2->scenePrimitive.type)
                     {

                         case PrimitiveType::PRIMITIVE_CONE:
                         {
                             //making the cone
                             m_coneI = std::unique_ptr<Cone_Implicit> (new Cone_Implicit());
                             tShadow = m_coneI->Intersect(localRay);

                             break;
                         }

                         case PrimitiveType::PRIMITIVE_CUBE:
                         {
                             //making the cube



                             m_cubeI = std::unique_ptr<Cube_Implicit> (new Cube_Implicit());
                             tShadow = m_cubeI->Intersect(localRay);


                             break;
                         }


                         case PrimitiveType::PRIMITIVE_SPHERE:
                         {
                             //making the sphere

                             float radius = 0.5;
                             m_sphereI = std::unique_ptr<Sphere_Implicit> (new Sphere_Implicit(radius));
                             tShadow = m_sphereI->Intersect(localRay);


                             break;
                         }


                         case PrimitiveType::PRIMITIVE_CYLINDER:
                         {
                             //making the cylinder
                             m_cylinderI = std::unique_ptr<Cylinder_Implicit> (new Cylinder_Implicit());
                             tShadow = m_cylinderI->Intersect(localRay);


                             break;
                         }

                     }//switch



                     //If object is in shadow, break loop and do not calculate color (i.e color = {0, 0, 0})
                     if (tShadow >= 0 && tShadow < att) {
                         isInShadow = true;
                         break;
                     }
                 }


         }

         //If object is not in shadow, calculate color
         if (isInShadow == false)
         {



              //direction of light
              //glm::vec3 L = -glm::normalize(glm::vec3(it->dir));     //glm::vec4(it->pos) - intersectionPos;
              glm::vec3 n = glm::normalize(norm);

              //0-1 whether or not normal is pointing at light
              float val = glm::dot(n, L);

              //clamping to zero
              if(val < 0)
              {
                  val = 0;
              }



             //Adding the diffuse color
             if(prim.material.textureMap.isUsed && settings.useTextureMapping)
             {

                  UV uv;
                  uv.U = 0;
                  uv.V = 0;


                  switch (is)
                  {
                     case 1:
                     {
                         uv = m_coneI->getUV(locIntersect);

                         break;
                     }

                     case 2:
                     {
                         uv = m_cubeI->getUV(locIntersect, textureList[prim.num].width(), textureList[prim.num].height());

                         break;
                     }

                     case 3:
                     {
                         uv = m_sphereI->getUV(locIntersect);

                         break;
                     }

                     case 4:
                     {
                         uv = m_cylinderI->getUV(locIntersect);

                         break;
                     }

                     default:
                     {}
                  }


                  float s;
                  float t;



                  s = static_cast<int>(uv.U * prim.material.textureMap.repeatU * textureList[prim.num].width()) % textureList[prim.num].width();
                  t = static_cast<int>(uv.V * prim.material.textureMap.repeatV * textureList[prim.num].height()) % textureList[prim.num].height();






                 QColor c = textureList[prim.num].pixelColor(s, t);
                 glm::vec4 colir = glm::vec4(c.red()/255.f, c.green()/255.f, c.blue()/255.f, c.alpha()/255.f);

                 glm::vec4 c2c = (colir * val);
                 glm::vec4 dif = m_global.kd * prim.material.cDiffuse * val;



                 if(prim.material.shininess > 0)
                 {

                     glm::vec3 V = glm::normalize( eyeInWorld - glm::vec3(intersectionPos));
                     glm::vec3 R = glm::normalize((glm::reflect(-L, n)));

                     float ma = glm::dot(R, V);

                     ma = std::min(std::max(0.f, ma), 1.f);

                     //adding specular term
                     glm::vec4 gt = (m_global.ks * prim.material.cSpecular * std::pow(ma, prim.material.shininess));

                     //accumulater += gt;
                     c2c += gt;
                     dif += gt;


                 }



                 //accounting for the color of the light
                 c2c[0] *= it->color.r;
                 c2c[1] *= it->color.g;
                 c2c[2] *= it->color.b;
                 c2c[3] *= it->color.a;

                 dif *= it->color;


                 //<--------------this is where i will account for attenuation
                 if(it->type == LightType::LIGHT_POINT)
                 {
                    c2c *= std::min(1/(it->function.x + (it->function.y*att) + (it->function.z*att*att)), 1.f);
                    dif *= std::min(1/(it->function.x + (it->function.y*att) + (it->function.z*att*att)), 1.f);
                 }



                 RGBA recCol;

                 if (m_recursionLimit > 0 && m_global.ks > 0 && (prim.material.cReflective.x > 0 || prim.material.cReflective.y > 0 || prim.material.cReflective.z > 0 || prim.material.cReflective.w > 0))
                 {
                     //glm::vec4 offset = glm::vec4(0.001f * norm, 0.0);
                     struct Ray recur;
                     recur.orig = glm::vec3(intersectionPos);
                     m_recursionLimit -= 1;
                     glm::vec3 e = glm::vec3(intersectionPos);
                     recCol = rend(recur, localRay, canvas, x, y, glm::reflect(glm::normalize(directionInWorld), glm::normalize(norm)), e, textureList);


                     dif[0] += (m_global.ks * prim.material.cReflective.x * (recCol.r/255.f));
                     dif[1] += (m_global.ks * prim.material.cReflective.y * (recCol.g/255.f));
                     dif[2] += (m_global.ks * prim.material.cReflective.z * (recCol.b/255.f));
                     dif[3] += (m_global.ks * prim.material.cReflective.w * (recCol.a/255.f));


                 }


                 accumulaterCurrentLight = ((1 - prim.material.blend) * dif) + (prim.material.blend * c2c);



             }
             else
             {
                 accumulaterCurrentLight += m_global.kd * prim.material.cDiffuse * val;




                 if(prim.material.shininess > 0)
                 {

                     glm::vec3 V = glm::normalize( eyeInWorld - glm::vec3(intersectionPos));
                     glm::vec3 R = glm::normalize((glm::reflect(-L, n)));

                     float ma = glm::dot(R, V);

                     ma = std::min(std::max(0.f, ma), 1.f);

                     //adding specular term
                     glm::vec4 gt = (m_global.ks * prim.material.cSpecular * std::pow(ma, prim.material.shininess));



                     accumulaterCurrentLight += gt;


                 }




                 //accounting for the color of the light

                 accumulaterCurrentLight[0] *= it->color.r;
                 accumulaterCurrentLight[1] *= it->color.g;
                 accumulaterCurrentLight[2] *= it->color.b;
                 accumulaterCurrentLight[3] *= it->color.a;



                 //<--------------this is where i will account for attenuation
                 if(it->type == LightType::LIGHT_POINT)
                 {
                    accumulaterCurrentLight *= std::min(1/(it->function.x + (it->function.y*att) + (it->function.z*att*att)), 1.f);
                 }




                  RGBA recCol;

                  if (m_recursionLimit > 0 && m_global.ks > 0 && (prim.material.cReflective.x > 0 || prim.material.cReflective.y > 0 || prim.material.cReflective.z > 0 || prim.material.cReflective.w > 0))
                  {
                      //glm::vec4 offset = glm::vec4(0.001f * norm, 0.0);
                      struct Ray recur;
                      recur.orig = glm::vec3(intersectionPos);
                      m_recursionLimit -= 1;
                      glm::vec3 e = glm::vec3(intersectionPos);
                      recCol = rend(recur, localRay, canvas, x, y, glm::reflect(glm::normalize(directionInWorld), glm::normalize(norm)), e, textureList);


                      accumulaterCurrentLight[0] += (m_global.ks * prim.material.cReflective.x * (recCol.r/255.f));
                      accumulaterCurrentLight[1] += (m_global.ks * prim.material.cReflective.y * (recCol.g/255.f));
                      accumulaterCurrentLight[2] += (m_global.ks * prim.material.cReflective.z * (recCol.b/255.f));
                      accumulaterCurrentLight[3] += (m_global.ks * prim.material.cReflective.w * (recCol.a/255.f));


                  }
             }
          }


         accumulater[0] += accumulaterCurrentLight[0];
         accumulater[1] += accumulaterCurrentLight[1];
         accumulater[2] += accumulaterCurrentLight[2];
         accumulater[3] += accumulaterCurrentLight[3];
         accumulaterCurrentLight = glm::vec4(0, 0, 0, 0);


     }




        // adding the ambient color        

        accumulater[0] += prim.material.cAmbient.x * m_global.ka;
        accumulater[1] += prim.material.cAmbient.y * m_global.ka;
        accumulater[2] += prim.material.cAmbient.z * m_global.ka;
        accumulater[3] += prim.material.cAmbient.w * m_global.ka;


        accumulater[0] = std::min(std::max(0.f, accumulater[0]), 1.f);
        accumulater[1] = std::min(std::max(0.f, accumulater[1]), 1.f);
        accumulater[2] = std::min(std::max(0.f, accumulater[2]), 1.f);
        accumulater[3] = std::min(std::max(0.f, accumulater[3]), 1.f);


        //setting the canvas color
        c = RGBA(accumulater[0]*255, accumulater[1]*255, accumulater[2]*255, accumulater[3]*255);


    }
    else
    {
        m_recursionLimit = 0;
    }


    return c;
}


RGBA RayScene::rend(struct Ray ray, struct Ray localRay, Canvas2D *canvas, int x, int y, glm::vec3 directionInWorld, glm::vec3 eyeInWorld, QImage textureList[])
{
    //calculate dir
    float shortestT = std::numeric_limits<float>::max();
    CS123ScenePrimitive prim;
    glm::vec3 norm;


    //other attempt


    ray.dir = directionInWorld;
    int is;
    struct Ray r;
    glm::vec3 normal;




    //for every item in the scene
    for (std::list<item>::iterator it = m_list.begin(); it != m_list.end(); ++it)
    {


        localRay.dir = worldToObjectSpaceDir(it->transformations, ray.dir);
        localRay.orig = worldToObjectSpace(it->transformations, ray.orig);




        switch (it->scenePrimitive.type)
        {

            case PrimitiveType::PRIMITIVE_CONE:
            {
                //making the cone
                m_coneI = std::unique_ptr<Cone_Implicit> (new Cone_Implicit());
                float tC = m_coneI->Intersect(localRay);
                if(tC!= -1)
                {

                    if(shortestT > tC)
                    {
                        normal = m_coneI->intersectNormal(localRay.orig+ tC * localRay.dir);
                        shortestT = tC;
                        norm = normalVectorToWorld(it->transformations, normal);
                        prim = it->scenePrimitive;
                        is = 1;
                        r = localRay;
                    }

                }
                break;
            }

            case PrimitiveType::PRIMITIVE_CUBE:
            {
                //making the cube



                m_cubeI = std::unique_ptr<Cube_Implicit> (new Cube_Implicit());
                float tC = m_cubeI->Intersect(localRay);
                if(tC != -1)
                {


                    if(shortestT > tC)
                    {
                        normal = m_cubeI->intersectNormal(localRay.orig+ tC * localRay.dir);
                        shortestT = tC;
                        norm = normalVectorToWorld(it->transformations, normal);
                        prim = it->scenePrimitive;
                        is = 2;
                        r = localRay;
                    }

                }

                break;
            }


            case PrimitiveType::PRIMITIVE_SPHERE:
            {
                //making the sphere

                float radius = 0.5;
                m_sphereI = std::unique_ptr<Sphere_Implicit> (new Sphere_Implicit(radius));
                float tC = m_sphereI->Intersect(localRay);
                if(tC != -1)
                {


                    if(shortestT > tC)
                    {
                        normal = m_sphereI->intersectNormal(localRay.orig+ tC * localRay.dir);
                        shortestT = tC;
                        norm = normalVectorToWorld(it->transformations, normal);
                        prim = it->scenePrimitive;
                        is = 3;
                        r = localRay;
                    }

                }

                break;
            }


            case PrimitiveType::PRIMITIVE_CYLINDER:
            {
                //making the cylinder
                m_cylinderI = std::unique_ptr<Cylinder_Implicit> (new Cylinder_Implicit());
                float tC = m_cylinderI->Intersect(localRay);
                if(tC != -1)
                {


                    if(shortestT > tC)
                    {
                        normal = m_cylinderI->intersectNormal(localRay.orig+ tC * localRay.dir);
                        shortestT = tC;
                        norm = normalVectorToWorld(it->transformations, normal);
                        prim = it->scenePrimitive;
                        is = 4;
                        r = localRay;
                    }


                }

                break;
            }

        }//switch


    }//for Loop


    //global intersection position
    glm::vec4 intersectionPos = glm::vec4(ray.orig + shortestT * ray.dir, 0.f);

    glm::vec4 offset1 = glm::vec4(0.001f * norm, 0.0);
    //glm::vec3 offset2 = glm::vec3(0.001f * normal);
    localRay.orig = glm::vec3(r.orig + shortestT * r.dir);//+offset2;
    localRay.dir = normal;

    return illuminate(shortestT, localRay, norm, canvas, prim, x, y, ray, directionInWorld, eyeInWorld, intersectionPos+offset1, textureList, is);
}







void RayScene::renderScene(Canvas2D *canvas, Camera *camera)
{

    struct Ray ray;
    struct Ray localRay;

    //std::cout << m_lightList.size() << std::endl;


    m_recursionLimit = 0;



    m_width = canvas->width();
    m_height = canvas->height();
    glm::vec3 eyeInCamera = glm::vec3(0, 0, 0);
    glm::mat4x4 cameraViewMatrix = camera->getViewMatrix();
    glm::mat4x4 cameraScaleMatrix = camera->getScaleMatrix();

    glm::vec3 eyeInWorld = cameraToWorldSpace(cameraViewMatrix, cameraScaleMatrix, eyeInCamera);

    ray.orig = eyeInWorld;
    QImage textureList[m_list.size()];


    //for every item in the scene
    int i = 0;
    for (std::list<item>::iterator it = m_list.begin(); it != m_list.end(); ++it, i++)
    {
        if(it->scenePrimitive.material.textureMap.isUsed)
        {
            QString qstr = QString::fromStdString(it->scenePrimitive.material.textureMap.filename);
            QImage img = QImage(qstr, 0);
            it->scenePrimitive.num = i;
            textureList[i] = img;
        }

    }





    //for every pixel
    for (int x = 0; x < m_width ; x++)
    {
        for (int y = 0; y < m_height ; y++)
        {

            glm::vec3 cameraSpacePoint = screenToCameraSpace(x, y, m_width, m_height);
            glm::vec3 worldSpacePoint = cameraToWorldSpace(cameraViewMatrix, cameraScaleMatrix, cameraSpacePoint);
            glm::vec3 directionInWorld = (worldSpacePoint - eyeInWorld);


            if (settings.useReflection)
            {
                    m_recursionLimit = 3;
            }
            else
            {
                m_recursionLimit = 0;
            }


            canvas->data()[get_index(y, x, m_width)] = rend(ray, localRay, canvas, x, y, directionInWorld, eyeInWorld, textureList);
        }
    }
}






RayScene::~RayScene()
{
}

