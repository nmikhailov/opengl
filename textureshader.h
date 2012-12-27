#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include <QGLShaderProgram>
#include <QGLBuffer>

class TextureShader : public QObject {
    Q_OBJECT
public:
    TextureShader(const QString& vertex, const QString& pixel);
    ~TextureShader();

    void render(GLuint id);
protected:
    QGLShaderProgram * m_shader;
    QGLBuffer * m_buff;
};

#endif // TEXTURESHADER_H
