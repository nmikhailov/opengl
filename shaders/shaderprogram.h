#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QObject>
#include <QGLContext>
#include <QGLShaderProgram>
#include <QGLBuffer>

#include <memory>

/*
 * Abstract shader program class
 * Manages data supplying to opengl shader
 */

//class ShaderManager;

class ShaderProgram : public QObject {
    Q_OBJECT
public:
    ShaderProgram(const ShaderProgram& program);
    virtual ~ShaderProgram();

    virtual bool bind(); // Binds shader program to opengl context

    // Index buffer
    virtual void setIndexBuffer(QGLBuffer buff);

    // Handle projection + transformation matrices
    virtual void setPTMatrix(const QMatrix4x4 & proj) = 0;

protected:
    friend class ShaderManager;

    ShaderProgram(QGLContext * context);
    virtual void init(QString vert, QString frag);

    std::shared_ptr<QGLShaderProgram> m_program;
    QGLContext * m_context;
};

#endif // SHADERPROGRAM_H
