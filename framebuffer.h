#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QObject>
#include <QSize>
#include <QtOpenGL>

class Framebuffer : public QObject {
    Q_OBJECT
public:
    Framebuffer(const QSize &size, bool texture = true);
    ~Framebuffer();

    GLuint id() const;
    QSize size() const;

    void bind() const;

    GLuint textureBuffer() const;
    GLuint depthBuffer() const;

    bool textureBufferEnabled() const;

private:
    void init();
    void initDepthTexture();
    void initTexture();

    GLuint m_texture, m_depth, m_id;
    QSize m_size;
    bool m_texture_enabled;
};

#endif // FRAMEBUFFER_H
