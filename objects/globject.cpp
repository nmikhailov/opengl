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
