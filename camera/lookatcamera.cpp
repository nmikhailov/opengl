#include "lookatcamera.h"

#include <QMatrix4x4>
#include <QVector3D>

LookAtCamera::LookAtCamera(MatrixStackManager *msm) : Camera(msm) {
    loadDefaults();
}

void LookAtCamera::loadDefaults() {
    m_pos = QVector3D(0, 0, 0);
    m_look_at = QVector3D(0, 0, 0);
    m_normal = QVector3D(0, 0, 1);
    m_angle = 45;
}

void LookAtCamera::setNormal(const QVector3D &normal) {
    m_normal = normal;
}

QVector3D LookAtCamera::normal() const {
    return m_normal;
}

void LookAtCamera::setLookAt(const QVector3D &lookAt) {
    m_look_at = lookAt;
}

QVector3D LookAtCamera::lookAt() const {
    return m_look_at;
}

void LookAtCamera::setAngle(double angle) {
    m_angle = angle;
}

double LookAtCamera::angle() const {
    return m_angle;
}

void LookAtCamera::apply() const {
    QMatrix4x4 m;
    m.lookAt(m_pos, m_look_at, m_normal);

    m_msmanager->setViewMatrix(m);
}
