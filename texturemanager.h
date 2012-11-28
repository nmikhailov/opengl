#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QGLContext>
#include <QtOpenGL>
#include <texture.h>

#include <map>

class TextureManager {
public:
    TextureManager(QGLContext * context);
    virtual ~TextureManager();

    Texture* createTexture(const QSize &size);
    void freeTexture(Texture* texture);

    GLuint loadTexture(const QImage & img);
    void unloadTexture(GLuint id);

    GLuint getTextureByName(const QString &file_name);
private:
    std::map<QString, GLuint> m_file2id;
    QGLContext * m_context;
};

#endif // TEXTUREMANAGER_H
