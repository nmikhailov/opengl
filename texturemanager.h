#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QGLWidget>
#include <QtOpenGL>

class TextureManager {
public:
    TextureManager(QGLWidget * widget);
    virtual ~TextureManager();

    GLuint loadTexture(const QImage & img);
    void unloadTexture(GLuint id);
private:
    QGLWidget * m_widget;
};

#endif // TEXTUREMANAGER_H
