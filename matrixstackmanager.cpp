#include "matrixstackmanager.h"
#include <QtOpenGL>

MatrixStackManager::MatrixStackManager(QGLShaderProgram *sh_program) {
    m_sh_program = sh_program;
    clear();
}

void MatrixStackManager::push() {
    m_stack.push_back(Matrix());
}

void MatrixStackManager::pop() {
    m_stack.pop_back();
}

const MatrixStackManager::Matrix &MatrixStackManager::top() const {
    return m_stack.back();
}

MatrixStackManager::Matrix &MatrixStackManager::top() {
    return m_stack.back();
}

MatrixStackManager::Matrix MatrixStackManager::projectionMatrix() const {
    return m_projection;
}

void MatrixStackManager::setProjectionMatrix(const MatrixStackManager::Matrix &m) {
    m_projection = m;
}

void MatrixStackManager::setPerspective(double aspectRatio, double angle, double near, double far) {
    m_projection.perspective(angle, aspectRatio, near, far);
}

void MatrixStackManager::apply() {
    Matrix res = m_projection;
    for(Matrix &m : m_stack) {
        res *= m;
    }
    // opengl 1
    //glLoadIdentity();
    //glMultMatrixd(res.constData());
    // opengl (3.3+, ES 2)
    // pass to shader
    m_sh_program->setUniformValue("proj", res);
}

void MatrixStackManager::clear() {
    m_stack.clear();
    m_stack.push_back(Matrix());

    m_projection = Matrix();
    // opengl 1
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}
