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

const ShaderManager *ContextManager::shaderManager() const {
    return m_shman;
}

ShaderManager *ContextManager::shaderManager() {
    return m_shman;
}

void ContextManager::setShaderManager(ShaderManager *sh_program) {
    m_shman = sh_program;
}
