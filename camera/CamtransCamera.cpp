/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"


CamtransCamera::CamtransCamera()
{
    setClip(1, 30);
    setHeightAngle(60);
    setAspectRatio(1);


    orientLook(glm::vec4(2, 2, 2, 1.0), glm::vec4(-2.0, -2.0, -2.0, 0.0), glm::vec4(0, 1, 0, 0.0));

}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    //return m_scaleMatrix*m_perspectiveTransformation;
    return m_perspectiveTransformation*m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
//    return m_translationMatrix*m_rotationMatrix;
    return m_rotationMatrix*m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    return m_up;
}
glm::vec4 CamtransCamera::getU() const {
    return m_u;
}
glm::vec4 CamtransCamera::getV() const {
    return m_v;
}
glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;

    m_up = up;

    m_w = glm::normalize(glm::vec4(-look[0], -look[1], -look[2], 0.0f));
    m_v = glm::normalize(up - glm::dot(up, m_w)*m_w);
    m_u = glm::normalize(glm::vec4(glm::cross(glm::vec3(m_v[0], m_v[1], m_v[2]), glm::vec3(m_w[0], m_w[1], m_w[2])), 0.0));




    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {


    float mh = (m_far*glm::tan(glm::radians(h)/2))*2.0;
    float mw = m_aspectRatio * mh;

    m_thetaW = glm::atan(mw/(m_far)/2.f)*2;
    m_thetaH = glm::radians(h);





    updateProjectionMatrix();
}


void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye += v;

    updateViewMatrix();
}


void CamtransCamera::rotateW(float degrees) {
    glm::vec4 u0 = m_u;
    glm::vec4 v0 = m_v;
    glm::vec4 w0 = m_w;


    m_w = w0;


    m_v = -u0 * static_cast<float>(sin(glm::radians(degrees))) +
          v0 * static_cast<float>(cos(glm::radians(degrees)));


    m_u = u0 * static_cast<float>(cos(glm::radians(degrees))) +
          v0 * static_cast<float>(sin(glm::radians(degrees)));

    updateViewMatrix();

}

void CamtransCamera::rotateU(float degrees) {
    glm::vec4 u0 = m_u;
    glm::vec4 v0 = m_v;
    glm::vec4 w0 = m_w;


    m_u = u0;


    m_v = w0 * static_cast<float>(sin(glm::radians(degrees))) +
          v0 * static_cast<float>(cos(glm::radians(degrees)));


    m_w = w0 * static_cast<float>(cos(glm::radians(degrees))) -
          v0 * static_cast<float>(sin(glm::radians(degrees)));

    updateViewMatrix();

}

void CamtransCamera::rotateV(float degrees) {
    glm::vec4 u0 = m_u;
    glm::vec4 v0 = m_v;
    glm::vec4 w0 = m_w;


    m_v = v0;


    m_w = u0 * static_cast<float>(sin(glm::radians(degrees))) +
          w0 * static_cast<float>(cos(glm::radians(degrees)));


    m_u = u0 * static_cast<float>(cos(glm::radians(degrees))) -
          w0 * static_cast<float>(sin(glm::radians(degrees)));

    updateViewMatrix();

}



void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}



void CamtransCamera::updateProjectionMatrix()
{
    updatePerspectiveMatrix();
    updateScaleMatrix();

}

void CamtransCamera::updatePerspectiveMatrix()
{

    float c = -(m_near/ m_far);

    m_perspectiveTransformation = glm::mat4(1,  0,  0,           0,
                                            0,  1,  0,           0,
                                            0,  0,  (-1/(c+1)),  -1,
                                            0,  0,  (c/(c+1)),   0);
}

void CamtransCamera::updateScaleMatrix()
{
    m_scaleMatrix = glm::mat4(1/(m_far*tan(((m_thetaW/2)))),  0,                              0,        0,
                              0,                              1/(m_far*tan(((m_thetaH/2)))),  0,        0,
                              0,                              0,                              1/m_far,  0,
                              0,                              0,                              0,        1);
}

void CamtransCamera::updateViewMatrix()
{

    updateRotationMatrix();
    updateTranslationMatrix();

}

void CamtransCamera::updateRotationMatrix()
{
    m_rotationMatrix = glm::mat4(m_u[0], m_v[0], m_w[0], 0,
                                 m_u[1], m_v[1], m_w[1], 0,
                                 m_u[2], m_v[2], m_w[2], 0,
                                 0,      0,      0,      1 );
}

void CamtransCamera::updateTranslationMatrix()
{
    glm::vec3 p = glm::vec3(-getPosition()[0], -getPosition()[1], -getPosition()[2]);
    m_translationMatrix = glm::mat4(1,    0,    0,    0,
                                    0,    1,    0,    0,
                                    0,    0,    1,    0,
                                    p[0], p[1], p[2], 1);
}
