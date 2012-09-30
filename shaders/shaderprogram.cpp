#include "shaderprogram.h"
#include <QFile>


ShaderProgram::ShaderProgram(QGLContext * context) {
    m_program = new QGLShaderProgram(context);
    m_context = context;
}

ShaderProgram::~ShaderProgram() {
    delete m_program;
}

void ShaderProgram::init(QString vert, QString frag) {
    QString prefix = ":/s/shaders/";

    m_program->addShaderFromSourceFile(QGLShader::Vertex, prefix + vert);
    m_program->addShaderFromSourceFile(QGLShader::Fragment, prefix + frag);
    m_program->link();
}


bool ShaderProgram::bind() {
    return m_program->bind();
}

void ShaderProgram::setIndexBuffer(QGLBuffer &buff) {
    buff.bind();
}
