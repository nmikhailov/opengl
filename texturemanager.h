#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QGLContext>
#include <QtOpenGL>
#include <QRect>

#include <map>

#include <texture.h>

class TextureManager {
public:
    TextureManager(QGLContext * context);
    virtual ~TextureManager();

    Texture* createTexture(const QSize &size);
    void freeTexture(Texture* texture);

    GLuint loadTexture(const QImage & img, int type);
    void unloadTexture(GLuint id);

    GLuint getTextureByName(Texture tex);

    GLuint genFBTexture(int width, int heigth);
    GLuint genFBDepthTexture(int width, int heigth);

    GLuint genDepthFramebuffer(const QSize &size, GLuint &tex_id);

private:
    std::map<QString, GLuint> m_file2id;
    QGLContext * m_context;
};

#endif // TEXTUREMANAGER_H
