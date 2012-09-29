#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QGLContext>
#include <QtOpenGL>

class TextureManager {
public:
    TextureManager(QGLContext * context);
    virtual ~TextureManager();

    GLuint loadTexture(const QImage & img);
    void unloadTexture(GLuint id);

private:
    QGLContext * m_context;
};

#endif // TEXTUREMANAGER_H
