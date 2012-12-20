#include <QtOpenGL>
#include "globject.h"


GLObject::GLObject() {
    m_renderer = nullptr;
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

GLRenderer *GLObject::renderer() const {
    return m_renderer;
}

void GLObject::setRenderer(GLRenderer *renderer) {
    m_renderer = renderer;
}

