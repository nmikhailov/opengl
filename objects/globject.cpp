#include <QtOpenGL>
#include "globject.h"


GLObject::GLObject(TextureManager *context, MatrixStackManager * ms_manager) {
    m_position = QVector3D(0, 0, 0);
    m_scale = QVector3D(1, 1, 1);
    m_rotation = QVector3D(0, 0, 0);
    m_texman = context;
    m_msmanager = ms_manager;
}

GLObject::~GLObject() {
}

void GLObject::draw() const {
    glBindTexture(GL_TEXTURE_2D, 0);


    m_msmanager->push();
    m_msmanager->top().translate(m_position.x(), m_position.y(), m_position.z());

    m_msmanager->top().rotate(m_rotation.x(), 1, 0, 0);
    m_msmanager->top().rotate(m_rotation.y(), 0, 1, 0);
    m_msmanager->top().rotate(m_rotation.z(), 0, 0, 1);

    m_msmanager->top().scale(m_scale.x(), m_scale.y(), m_scale.z());

    m_msmanager->apply();
    _draw();

    m_msmanager->pop();
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
