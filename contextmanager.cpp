#include "contextmanager.h"

ContextManager::ContextManager(QGLContext *context) {
    m_context = context;
}


const TextureManager *ContextManager::textureManager() const {
    return m_texman;
}

TextureManager *ContextManager::textureManager() {
    return m_texman;
}

void ContextManager::setTextureManager(TextureManager *texman) {
    m_texman = texman;
}

const MatrixStackManager *ContextManager::matrixStackManager() const {
    return m_msmanager;
}

MatrixStackManager *ContextManager::matrixStackManager() {
    return m_msmanager;
}

void ContextManager::setMatrixStackManager(MatrixStackManager *msmanager) {
    m_msmanager = msmanager;
}

const QGLShaderProgram *ContextManager::shaderProgram() const {
    return m_sh_program;
}

QGLShaderProgram *ContextManager::shaderProgram() {
    return m_sh_program;
}

void ContextManager::setShaderProgram(QGLShaderProgram *sh_program) {
    m_sh_program = sh_program;
}
