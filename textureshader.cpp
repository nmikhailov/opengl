#include "textureshader.h"

namespace {
const GLfloat vertices[] = {
    -1, -1,
     1, -1,
    -1,  1,

    -1,  1,
     1, -1,
     1,  1,
};
}

TextureShader::TextureShader(const QString &vertex, const QString &pixel) {
    m_shader = new QGLShaderProgram();
    m_shader->addShaderFromSourceFile(QGLShader::Vertex, ":/s/shaders/" + vertex);
    m_shader->addShaderFromSourceFile(QGLShader::Fragment, ":/s/shaders/" + pixel);
    m_shader->link();

    m_buff = new QGLBuffer();
    m_buff->create();
    m_buff->bind();
    m_buff->allocate(vertices, sizeof(vertices));
}

TextureShader::~TextureShader() {
    if (m_shader) {
        m_shader->release();
        delete m_shader;
    }
    if (m_buff) {
        m_buff->release();
        delete m_buff;
    }
}

void TextureShader::render(GLuint id) {
    m_shader->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    m_shader->setUniformValue("texture", 0);

    m_buff->bind();
    m_shader->setAttributeBuffer("uv", GL_FLOAT, 0, 2);
    m_shader->enableAttributeArray("uv");

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

