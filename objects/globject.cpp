#include <QtOpenGL>
#include "globject.h"


GLObject::GLObject(Scene *scene) {
    m_position = QVector3D(0, 0, 0);
    m_scale = QVector3D(1, 1, 1);
    m_rotation = QQuaternion();
    m_scene = scene;
}

GLObject::~GLObject() {
}

void GLObject::draw() const {
    MatrixStackManager * msm = m_context->matrixStackManager();

    msm->push();
    msm->top().translate(m_position.x(), m_position.y(), m_position.z());
    msm->top().rotate(m_rotation);
    msm->top().scale(m_scale.x(), m_scale.y(), m_scale.z());

    msm->apply();
    _draw();

    msm->pop();
}


QVector3D GLObject::position() const {
    return m_position;
}

void GLObject::setPosition(const QVector3D &vec) {
    m_position =  vec;
}

QQuaternion GLObject::rotation() const {
    return m_rotation;
}

void GLObject::setRotation(const QQuaternion &vec) {
    m_rotation = vec;
}

QVector3D GLObject::scale() const {
    return m_scale;
}

void GLObject::setScale(const QVector3D &vec) {
    m_scale = vec;
}
