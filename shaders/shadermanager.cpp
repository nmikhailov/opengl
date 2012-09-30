#include "shadermanager.h"

#include <boost/fusion/include/for_each.hpp>


ShaderManager::ShaderManager(QGLContext *context) {
    m_context = context;
    setActiveShader<default_shader>();
}

ShaderManager::~ShaderManager() {
    boost::fusion::for_each(m_programs, release());
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
