#include "shaderprogram.h"
#include <QFile>


ShaderProgram::ShaderProgram(QGLContext * context) :
    m_program(new QGLShaderProgram(context)) {
    m_context = context;
}

ShaderProgram::~ShaderProgram() {
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

void ShaderProgram::setIndexBuffer(const QGLBuffer &buff) {
    QGLBuffer b(buff);
    b.bind();
}

ShaderProgram::ShaderProgram(const ShaderProgram &program) :
    QObject() {
    this->m_context = program.m_context;
    this->m_program = program.m_program;
}
