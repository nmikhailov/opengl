#ifndef TEXTUREBLENDSHADER_H
#define TEXTUREBLENDSHADER_H

#include <QColor>
#include <QGLBuffer>
#include <QString>

#include "shaderprogram.h"

class TextureBlendShader : public ShaderProgram {
    Q_OBJECT
public:

    void setVertexBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setUVBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 2);

    void bindTexture(int tex_id, int val);

    void setModelMatrix(const QMatrix4x4 &proj);
    void setViewMatrix(const QMatrix4x4 & proj);
    void setProjectionMatrix(const QMatrix4x4 & proj);
protected:
    friend class ShaderManager;
    TextureBlendShader(QGLContext * context);
};

#endif // TEXTUREBLENDSHADER_H
