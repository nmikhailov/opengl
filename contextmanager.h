#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <QObject>
#include <QGLContext>
#include <QGLShaderProgram>
#include <QGLBuffer>

#include "texturemanager.h"
#include "matrixstackmanager.h"
#include "shaders/shadermanager.h"

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

    const ShaderManager* shaderManager() const;
    ShaderManager* shaderManager();
    void setShaderManager(ShaderManager * sh_program);

protected:
    QGLContext * m_context;
    TextureManager * m_texman = nullptr;
    MatrixStackManager * m_msmanager = nullptr;
    ShaderManager * m_shman = nullptr;
};

#endif // CONTEXTMANAGER_H
