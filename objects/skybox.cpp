#include "skybox.h"

Skybox::Skybox() {
    init();
}

Material Skybox::material() const {
    TextureInfo t("skybox.png", TextureInfo::T_CUBIC);
    Material m;
    m.setTexture(t);
    m.setType(Material::C_TEXTURE);

    return m;
}
namespace {
    GLfloat cube_vertices[] = {
      -1.0,  1.0,  1.0,
      -1.0, -1.0,  1.0,
       1.0, -1.0,  1.0,
       1.0,  1.0,  1.0,
      -1.0,  1.0, -1.0,
      -1.0, -1.0, -1.0,
       1.0, -1.0, -1.0,
       1.0,  1.0, -1.0,
    };

    GLuint cube_indices[] = {
      0, 1, 2, 3,
      3, 2, 6, 7,
      7, 6, 5, 4,
      4, 5, 1, 0,
      0, 3, 7, 4,
      1, 2, 6, 5,
    };
}

GLObject::BufferInfo Skybox::vertexBuffer() const {
    return GLObject::BufferInfo(true, GL_FLOAT, sizeof(cube_vertices) / sizeof(GLfloat), m_buff);
}

GLenum Skybox::primitiveType() const {
    return GL_TRIANGLES;
}

GLObject::BufferInfo Skybox::indexBuffer() const {
    return GLObject::BufferInfo(true, GL_UNSIGNED_INT, sizeof(cube_indices) / sizeof(GLuint), m_ind);
}

Rect Skybox::rect() const {
    return Rect();
}

void Skybox::init() {
    m_buff.create();
    m_buff.bind();
    m_buff.allocate(cube_vertices, sizeof(cube_indices));

    m_ind = QGLBuffer(QGLBuffer::IndexBuffer);
    m_ind.create();
    m_ind.bind();
    m_ind.allocate(cube_indices, sizeof(cube_indices));
}
