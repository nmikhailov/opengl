#ifndef GLDEPTHSHADER_H
#define GLDEPTHSHADER_H

#include <QColor>
#include <QGLBuffer>
#include <QString>
#include <memory>
#include <QGLShaderProgram>

#include "scene.h"
#include "objects/globject.h"
#include "material.h"
#include "objects/globject.h"

class GLObject;
class Scene;
class Material;

class GLDepthShader: public QObject {
    Q_OBJECT
public:
    void setVertexBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setIndexBuffer(QGLBuffer buff);

    void setModelMatrix(const QMatrix4x4 &proj);
    void setViewMatrix(const QMatrix4x4 & proj);
    void setProjectionMatrix(const QMatrix4x4 & proj);

    // New stuff
    void render(const GLObject* obj);

    bool bind();
    void release();
protected:
    friend class Scene;

    GLDepthShader(Scene *scene);
    virtual ~GLDepthShader();
    void init(QString vert, QString frag);

    Scene *m_scene;

    std::shared_ptr<QGLShaderProgram> m_program;
};


#endif // GLDEPTHSHADER_H
