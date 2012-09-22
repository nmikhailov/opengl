#include "camera.h"

#include <GL/glu.h>

Camera::Camera(const QVector3D &pos, const QVector3D &lookAt, const QVector3D &normal, double angle) {
    m_pos = pos;
    m_lookAt = lookAt;
    m_normal = normal;
    m_angle = angle;
}

void Camera::setPosition(const QVector3D &pos) {
    m_pos = pos;
}

QVector3D Camera::position() const {
    return m_pos;
}

void Camera::setNormal(const QVector3D &normal) {
    m_normal = normal;
}

QVector3D Camera::normal() const {
    return m_normal;
}

void Camera::setLookAt(const QVector3D &lookAt) {
    m_lookAt = lookAt;
}

QVector3D Camera::lookAt() const {
    return m_lookAt;
}

void Camera::setAngle(double angle) {
    m_angle = angle;
}

double Camera::angle() const {
    return m_angle;
}

void Camera::setAspectRatio(double ar) {
    m_aspect = ar;
}

void Camera::apply() const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle(), m_aspect, 0.1f, 100000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_pos.x(), m_pos.y(), m_pos.z(),
              m_lookAt.x(), m_lookAt.y(), m_lookAt.z(),
              m_normal.x(), m_normal.y(), m_normal.z());
}
