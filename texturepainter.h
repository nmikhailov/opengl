#ifndef TEXTUREPAINTER_H
#define TEXTUREPAINTER_H

#include <QGLBuffer>
#include <QGLShaderProgram>

class TexturePainter {
public:
    TexturePainter();
    virtual ~TexturePainter();

    void bind();
    void release();
    void renderTexture(GLuint texid);

protected:
    QGLShaderProgram m_shader;
    QGLBuffer m_uv;
};

#endif // TEXTUREPAINTER_H
