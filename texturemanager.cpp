#include "texturemanager.h"

TextureManager::TextureManager(QGLWidget *widget) {
    m_widget = widget;
}

void TextureManager::~TextureManager() {
}

GLuint TextureManager::loadTexture(const QImage &img) {
    m_widget->bindTexture(img);
}

void TextureManager::unloadTexture(GLuint id) {
    m_widget->deleteTexture(id);
}

