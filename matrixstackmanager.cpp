#include "matrixstackmanager.h"
#include <QtOpenGL>

MatrixStackManager::MatrixStackManager(QGLShaderProgram *sh_program) {
    m_sh_program = sh_program;
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

void MatrixStackManager::apply() {
    Matrix res;
    for(Matrix &m : m_stack) {
        res *= m;
    }
    // opengl 1
    glMultMatrixd(res.constData());
}
