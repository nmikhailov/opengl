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
    virtual void setModelMatrix(const QMatrix4x4 & proj) = 0;
    virtual void setViewMatrix(const QMatrix4x4 & proj) = 0;
    virtual void setProjectionMatrix(const QMatrix4x4 & proj) = 0;

    virtual void activated();
protected:
    friend class ShaderManager;

    ShaderProgram(Scene *context);
    virtual void init(QString vert, QString frag);

    std::shared_ptr<QGLShaderProgram> m_program;
    Scene *m_scene;
};

#endif // SHADERPROGRAM_H
