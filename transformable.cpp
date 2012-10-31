#include "transformable.h"


Transformable::Transformable() {
    m_position = QVector3D(0, 0, 0);
    m_scale = QVector3D(1, 1, 1);
    m_rotation = QQuaternion();
}

Transformable::~Transformable() {
}

QVector3D Transformable::position() const {
    return m_position;
}

void Transformable::setPosition(const QVector3D &vec) {
    m_position =  vec;
}

QQuaternion Transformable::rotation() const {
    return m_rotation;
}

void Transformable::setRotation(const QQuaternion &vec) {
    m_rotation = vec;
}

QVector3D Transformable::scale() const {
    return m_scale;
}

void Transformable::setScale(const QVector3D &vec) {
    m_scale = vec;
}

QMatrix4x4 Transformable::trMatrix() const {
    QMatrix4x4 mat;
    mat.rotate(m_rotation);
    mat.translate(m_position);

    mat.scale(m_scale);
    return mat;
}
