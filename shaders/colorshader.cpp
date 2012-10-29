#include "colorshader.h"

ColorShader::ColorShader(Scene *scene) : ShaderProgram(scene) {
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
    m_program->setAttributeBuffer("vert", type, 0, tupleSize);
    m_program->enableAttributeArray("vert");
}

void ColorShader::setColorBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("color", type, 0, tupleSize);
    m_program->enableAttributeArray("color");
}

void ColorShader::setNormalBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("norm_buf", type, 0, tupleSize);
    m_program->enableAttributeArray("norm_buf");
}

void ColorShader::setUVBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("uv_buf", type, 0, tupleSize);
    m_program->enableAttributeArray("uv_buf");
}

void ColorShader::bindTexture(GLuint id) {
    m_program->setUniformValue("tex", id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

void ColorShader::setModelMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("M", proj);
    m_program->setUniformValue("M_N", proj.normalMatrix());
}

void ColorShader::setViewMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("V", proj);
}

void ColorShader::setProjectionMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("P", proj);
}

void ColorShader::activated() {
    int id = 0;
    for (int i = 0; i < m_scene->lightCount(); i++) {
        LightSource *light = m_scene->light(i);

        if (m_scene->inLastSceneTree(light)) {
            QString name = QString("light[%1].").arg(id++);

            QVector3D pos = (m_scene->modelMatrix(light) * QVector4D(light->position(), 0)).toVector3D();

            m_program->setUniformValue(name + "diffuse", light->diffuseColor());
            m_program->setUniformValue(name + "position", pos);
            m_program->setUniformValue(name + "att", light->attenuation());
        }
    }
    m_program->setUniformValueArray("lightCnt", id);
}
