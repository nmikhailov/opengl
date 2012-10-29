#include "shaderprogram.h"
#include <QFile>


ShaderProgram::ShaderProgram(Scene * context) :
    m_program(new QGLShaderProgram(context)) {
    m_scene = scene;
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

void ShaderProgram::setIndexBuffer(QGLBuffer buff) {
    buff.bind();
}

void ShaderProgram::activated() {
}

ShaderProgram::ShaderProgram(const ShaderProgram &program) :
    QObject() {
    this->m_scene = program.m_scene;
    this->m_program = program.m_program;
}
