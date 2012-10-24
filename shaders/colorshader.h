#ifndef COLORSHADER_H
#define COLORSHADER_H

#include <QColor>
#include <QGLBuffer>
#include <QString>

#include "shaderprogram.h"

class ColorShader : public ShaderProgram {
    Q_OBJECT
public:
    enum EColorMode {CM_ONE_COLOR = 0, CM_COLOR_MAP = 1, CM_TEXTURE = 2};

    // Used when one color mode is enabled
    void setColor(const QColor & color);
    void setColorMode(EColorMode mode);
    
    void setVertexBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setColorBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setNormalBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);

    void setUVBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 2);
    void bindTexture(GLuint id);

    void setModelMatrix(const QMatrix4x4 &proj);
    void setViewMatrix(const QMatrix4x4 & proj);
    void setProjectionMatrix(const QMatrix4x4 & proj);
protected:
    friend class ShaderManager;
    ColorShader(QGLContext * context);
};

#endif // COLORSHADER_H
