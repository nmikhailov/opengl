#include "texturemanager.h"
#include <QtOpenGL>

TextureManager::TextureManager(QGLContext *context) {
    m_context = context;
}

TextureManager::~TextureManager() {
}

GLuint TextureManager::loadTexture(const QImage &img) {
    return m_context->bindTexture(img);
}

void TextureManager::unloadTexture(GLuint id) {
    m_context->deleteTexture(id);
}

GLuint TextureManager::getTextureByName(const QString &file_name) {
    if(m_file2id.find(file_name) == m_file2id.end()) {
        m_file2id[file_name] = loadTexture(QImage(file_name));
        qDebug() << "Requsted " << file_name;
    }
    return m_file2id[file_name];
}

