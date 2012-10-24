#include "matrixstackmanager.h"
#include <QtOpenGL>

MatrixStackManager::MatrixStackManager(ShaderManager *sh_program) {
    m_shman = sh_program;
    clear();

    connect(sh_program, SIGNAL(activeShaderChanged()), this, SLOT(apply()));
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

MatrixStackManager::Matrix MatrixStackManager::viewMatrix() const {
    return m_view;
}

void MatrixStackManager::setViewMatrix(const MatrixStackManager::Matrix &m) {
    m_view = m;
}

void MatrixStackManager::apply() {
    Matrix res;
    for(Matrix &m : m_stack) {
        res *= m;
    }

    m_shman->getActiveShader().setModelMatrix(res);
    m_shman->getActiveShader().setProjectionMatrix(m_projection);
    m_shman->getActiveShader().setViewMatrix(m_view);
}

void MatrixStackManager::clear() {
    m_stack.clear();
    m_stack.push_back(Matrix());

    m_projection = Matrix();
}
