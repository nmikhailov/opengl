#include "textureblendshader.h"

TextureBlendShader::TextureBlendShader(QGLContext *context) : ShaderProgram(context) {
    init("mtex_landscape.vert", "mtex_landscape.frag");
}

void TextureBlendShader::setVertexBuffer(const QGLBuffer &buff, GLenum type, int tupleSize) {
    QGLBuffer b(buff);
    b.bind();
    m_program->setAttributeBuffer(0, type, 0, tupleSize);
    m_program->enableAttributeArray(0);
}

void TextureBlendShader::setUVBuffer(const QGLBuffer &buff, GLenum type, int tupleSize) {
    QGLBuffer b(buff);
    b.bind();
    m_program->setAttributeBuffer(1, type, 0, tupleSize);
    m_program->enableAttributeArray(1);
}

void TextureBlendShader::setPTMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("proj", proj);
}

void TextureBlendShader::bindTexture(int tex_id, int val) {
    const char* names[] = {"tex1", "tex2", "tex3"};
    m_program->setUniformValue(names[tex_id], tex_id);
    glActiveTexture(GL_TEXTURE0 + tex_id);
    glBindTexture(GL_TEXTURE_2D, val);
}

void TextureBlendShader::setProp(float a, float b) {
    m_program->setUniformValue("prop", QVector2D(a, b));
}
