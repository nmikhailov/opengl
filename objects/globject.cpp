#include <QtOpenGL>
#include "globject.h"


GLObject::GLObject(ContextManager * context) {
    m_position = QVector3D(0, 0, 0);
    m_scale = QVector3D(1, 1, 1);
    m_rotation = QMatrix4x4();
    m_context = context;
}

GLObject::~GLObject() {
}

void GLObject::draw() const {
    MatrixStackManager * msm = m_context->matrixStackManager();

    msm->push();
    msm->top().translate(m_position.x(), m_position.y(), m_position.z());

    msm->top() *= m_rotation;

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

QMatrix4x4 GLObject::rotation() const {
    return m_rotation;
}

void GLObject::setRotation(const QVector3D &vec) {
    m_rotation.setToIdentity();
    m_rotation.rotate(vec.x(), 1, 0, 0);
    m_rotation.rotate(vec.y(), 0, 1, 0);
    m_rotation.rotate(vec.z(), 0, 0, 1);
}

void GLObject::setRotation(const QMatrix4x4 &mx) {
    m_rotation = mx;
}

void GLObject::rotateBy(double x, double y, double z) {
    QMatrix4x4 m;
    m.setToIdentity();
    m.rotate(x, 1, 0, 0);
    m.rotate(y, 0, 1, 0);
    m.rotate(z, 0, 0, 1);
    m_rotation = m * m_rotation;
}

QVector3D GLObject::scale() const {
    return m_scale;
}

void GLObject::setScale(const QVector3D &vec) {
    m_scale = vec;
}
