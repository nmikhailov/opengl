#include "shadermanager.h"


ShaderManager::ShaderManager(QGLContext *context) {
    m_context = context;
    setActiveShader<default_shader>();
}

ShaderManager::~ShaderManager() {
    // TODO: delete shaders
}

ShaderProgram &ShaderManager::getActiveShader() {
    return *m_active_shader;
}


void ShaderManager::activate(ShaderProgram *shader) {
    if(m_active_shader != shader) {
        m_active_shader = shader;
        m_active_shader->bind();
    }
}
