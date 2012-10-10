#include "colorshader.h"

ColorShader::ColorShader(QGLContext *context) : ShaderProgram(context) {
    init("v_main.vert", "f_main.frag");
}

void ColorShader::setColor(const QColor &color) {
    m_program->setUniformValue("cl_color", color);
}

void ColorShader::setColorMode(ColorShader::EColorMode mode) {
    m_program->setUniformValue("color_mode", (int)mode);
}

void ColorShader::setVertexBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(0, type, 0, tupleSize);
    m_program->enableAttributeArray(0);
}

void ColorShader::setColorBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(1, type, 0, tupleSize);
    m_program->enableAttributeArray(1);
}

void ColorShader::setNormalBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(3, type, 0, tupleSize);
    m_program->enableAttributeArray(3);
}

void ColorShader::setUVBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(2, type, 0, tupleSize);
    m_program->enableAttributeArray(2);
}

void ColorShader::bindTexture(GLuint id) {
    //m_program->setUniformValue("tex", id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

void ColorShader::setPTMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("proj", proj);
}
