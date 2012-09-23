#include "texturemanager.h"
#include <QtOpenGL>

TextureManager::TextureManager(QGLWidget *widget) {
    m_widget = widget;
}

TextureManager::~TextureManager() {
}

GLuint TextureManager::loadTexture(const QImage &img) {
//    glEnable(GL_TEXTURE_2D);


//    GLuint id;
//    QImage textureImage;
//    QImage tempImage(img);

//    textureImage = QGLWidget::convertToGLFormat(tempImage);

//    glGenTextures(1, &id );
//    glBindTexture(GL_TEXTURE_2D, id);

//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    return id;

    return m_widget->bindTexture(img);
}

void TextureManager::unloadTexture(GLuint id) {
    m_widget->deleteTexture(id);
}

