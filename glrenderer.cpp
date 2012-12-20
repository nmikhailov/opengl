#include "glrenderer.h"

void GLRenderer::init(QString vert, QString frag) {
    QString prefix = "/home/nsl/Study/s07/graphics/qt_labs/Lab_02/shaders/";

    m_program->addShaderFromSourceFile(QGLShader::Vertex, prefix + vert);
    m_program->addShaderFromSourceFile(QGLShader::Fragment, prefix + frag);
    m_program->link();
}


GLRenderer::~GLRenderer() {
}


GLRenderer::GLRenderer() : m_program(new QGLShaderProgram()) {
}
