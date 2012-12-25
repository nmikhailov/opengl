#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QGLContext>
#include <QtOpenGL>
#include <QRect>

#include <map>
#include <unordered_map>

#include <texture.h>

class TextureManager {
public:
    TextureManager(QGLContext * context);
    virtual ~TextureManager();

    TextureInfo* createTexture(const QSize &size);
    void freeTexture(TextureInfo* texture);

    void unloadTexture(GLuint id);

    GLuint getTexture(TextureInfo tex);

    GLuint genFBTexture(int width, int heigth);
    GLuint genFBDepthTexture(int width, int heigth);

    GLuint genDepthFramebuffer(const QSize &size, GLuint &tex_id);

private:
    GLuint loadTexture(const QImage & img, TextureInfo::T_TYPE type);

    std::unordered_map<TextureInfo, GLuint> m_cache;

    QGLContext * m_context;
};

#endif // TEXTUREMANAGER_H
