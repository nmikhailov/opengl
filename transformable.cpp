#include "transformable.h"

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
    mat.translate(m_position);
    mat.rotate(m_rotation);
    mat.scale(m_scale);
    return mat;
}


Transformable::Transformable() {
}

Transformable::~Transformable() {
}
