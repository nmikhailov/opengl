#include <QtOpenGL>
#include "globject.h"


GLObject::GLObject(Scene *scene) {
    m_scene = scene;
}

GLObject::~GLObject() {
}


GLObject::BufferInfo::BufferInfo() {
}

GLObject::BufferInfo::BufferInfo(bool enabled, GLenum type, int sz, QGLBuffer buff) {
    this->buff = buff;
    this->type = type;
    this->enabled = enabled;
    this->sz = sz;
}


GLObject::BufferInfo GLObject::indexBuffer() const {
    return BufferInfo(false);
}

GLObject::BufferInfo GLObject::normalBuffer() const {
    return BufferInfo(false);
}

GLObject::BufferInfo GLObject::texcoordBuffer() const {
    return BufferInfo(false);
}

