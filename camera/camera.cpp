#include "camera.h"
#include <QVector3D>

Camera::Camera(MatrixStackManager *msm) {
    m_msmanager = msm;
}

void Camera::setPosition(const QVector3D &pos) {
    m_pos = pos;
}

QVector3D Camera::position() const {
    return m_pos;
}

