#include <QtOpenGL>
#include "globject.h"


GLObject::GLObject(ContextManager * context) {
    m_position = QVector3D(0, 0, 0);
    m_scale = QVector3D(1, 1, 1);
    m_rotation = QVector3D(0, 0, 0);
    m_context = context;
}

GLObject::~GLObject() {
}

void GLObject::draw() const {
    MatrixStackManager * msm = m_context->matrixStackManager();

    msm->push();
    msm->top().translate(m_position.x(), m_position.y(), m_position.z());

    msm->top().rotate(m_rotation.x(), 1, 0, 0);
    msm->top().rotate(m_rotation.y(), 0, 1, 0);
    msm->top().rotate(m_rotation.z(), 0, 0, 1);

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

QVector3D GLObject::rotation() const {
    return m_rotation;
}

void GLObject::setRotation(const QVector3D &vec) {
    m_rotation = vec;
}

void GLObject::rotateBy(double x, double y, double z) {
    m_rotation += QVector3D(x, y, z);
}

QVector3D GLObject::scale() const {
    return m_scale;
}

void GLObject::setScale(const QVector3D &vec) {
    m_scale = vec;
}
