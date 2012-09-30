#ifndef COLORSHADER_H
#define COLORSHADER_H

#include <QColor>
#include <QGLBuffer>
#include <QString>

#include "shaderprogram.h"

class ColorShader : public ShaderProgram {
    Q_OBJECT
public:
    enum EColorMode {ONE_COLOR, COLOR_MAP};

    // Used when one color mode is enabled
    void setColor(const QColor & color);
    void setColorMode(EColorMode mode);
    
    void setVertexBuffer(QGLBuffer &buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setColorBuffer(QGLBuffer &buff, GLenum type = GL_FLOAT, int tupleSize = 3);

    void setPTMatrix(const QMatrix4x4 &proj);
protected:
    friend class ShaderManager;
    ColorShader(QGLContext * context);
    virtual ~ColorShader() {}
};

#endif // COLORSHADER_H
