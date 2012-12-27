#include "camera.h"
#include <QVector3D>


void Camera::setPosition(const QVector3D &pos) {
    m_pos = pos;
}

QVector3D Camera::position() const {
    return m_pos;
}

void Camera::setScreenSize(const QSize &rect) {
    m_screen_size = rect;
}

QSize Camera::screenSize() const {
    return m_screen_size;
}

Camera::Camera() {
    m_screen_size = QSize(1, 1);
}
