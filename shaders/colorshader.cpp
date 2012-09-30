#include "colorshader.h"

ColorShader::ColorShader(QGLContext *context) : ShaderProgram(context) {
    init("v_main.vert", "f_main.frag");
}

void ColorShader::setColor(const QColor &color) {
    m_program->setUniformValue("cl_color", color);
}

void ColorShader::setColorMode(ColorShader::EColorMode mode) {
    if(mode == ONE_COLOR) {
        m_program->setUniformValue("oneColor", true);
    } else {
        m_program->setUniformValue("oneColor", false);
    }
}

void ColorShader::setVertexBuffer(QGLBuffer &buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(0, type, 0, tupleSize);
    m_program->enableAttributeArray(0);
}

void ColorShader::setColorBuffer(QGLBuffer &buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer(1, type, 0, tupleSize);
    m_program->enableAttributeArray(1);
}

void ColorShader::setPTMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("proj", proj);
}
