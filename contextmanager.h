#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <QObject>
#include <QGLContext>
#include <QGLShaderProgram>
#include <QGLBuffer>

#include "texturemanager.h"
#include "matrixstackmanager.h"

/*
 * Provides access to some opengl stuff:
 * Textures, shaders, matrix stack
 */

class ContextManager : public QObject {
    Q_OBJECT
public:
    ContextManager(QGLContext * context);
    virtual ~ContextManager() {}
    
    const QGLContext* context() const;

    const TextureManager* textureManager() const;
    TextureManager* textureManager();
    void setTextureManager(TextureManager * texman);

    const MatrixStackManager* matrixStackManager() const;
    MatrixStackManager* matrixStackManager();
    void setMatrixStackManager(MatrixStackManager * msmanager);

    const QGLShaderProgram* shaderProgram() const;
    QGLShaderProgram* shaderProgram();
    void setShaderProgram(QGLShaderProgram * sh_program);

    enum LOCATIONS{LOC_VERTEX = 1, LOC_COLOR = 2};
protected:
    QGLContext * m_context;
    TextureManager * m_texman = nullptr;
    MatrixStackManager * m_msmanager = nullptr;
    QGLShaderProgram * m_sh_program = nullptr;
};

#endif // CONTEXTMANAGER_H
