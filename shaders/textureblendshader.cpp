#include "textureblendshader.h"

TextureBlendShader::TextureBlendShader(QGLContext *context) : ShaderProgram(context) {
    init("mtex_landscape.vert", "mtex_landscape.frag");
}

void TextureBlendShader::setVertexBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(0, type, 0, tupleSize);
    m_program->enableAttributeArray(0);
}

void TextureBlendShader::setUVBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(1, type, 0, tupleSize);
    m_program->enableAttributeArray(1);
}

void TextureBlendShader::setModelMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("proj", proj);
}

void TextureBlendShader::setViewMatrix(const QMatrix4x4 &proj) {
}

void TextureBlendShader::setProjectionMatrix(const QMatrix4x4 &proj) {
}

void TextureBlendShader::bindTexture(int tex_id, int val) {
    const char* names[] = {"tex1", "tex2", "tex3"};
    m_program->setUniformValue(names[tex_id], tex_id);
    glActiveTexture(GL_TEXTURE0 + tex_id);
    glBindTexture(GL_TEXTURE_2D, val);
}
