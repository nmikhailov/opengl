#include "camera.h"
#include <QVector3D>


void Camera::setPosition(const QVector3D &pos) {
    m_pos = pos;
}

QVector3D Camera::position() const {
    return m_pos;
}

void Camera::setScreenSize(const QVector2D &rect) {
    m_screen_size = rect;
}

QVector2D Camera::screenSize() const {
    return m_screen_size;
}

Camera::Camera() {
    m_screen_size = QVector2D(1, 1);
}
