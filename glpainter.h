#ifndef GLPAINTER_H
#define GLPAINTER_H

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

class GLPainter: public QObject {
    Q_OBJECT
public:
    enum EColorMode {CM_ONE_COLOR = 0, CM_COLOR_MAP = 1, CM_TEXTURE = 2};

    // Used when one color mode is enabled
    void setColor(const QColor & color);
    void setColorMode(EColorMode mode);
    
    void setVertexBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setColorBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setNormalBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setIndexBuffer(QGLBuffer buff);

    void setUVBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 2);
    void bindTexture(GLuint id);

    void setModelMatrix(const QMatrix4x4 &proj);
    void setViewMatrix(const QMatrix4x4 & proj);
    void setProjectionMatrix(const QMatrix4x4 & proj);

    // New stuff
    void updateLight(const std::map<LightSource*, QMatrix4x4> &lights);
    void render(const GLObject* obj);
    void setMaterial(const Material &m);

    bool bind();
    void release();
protected:
    friend class Scene;

    GLPainter(Scene *scene);
    virtual ~GLPainter();
    void init(QString vert, QString frag);

    Scene *m_scene;

    std::shared_ptr<QGLShaderProgram> m_program;
};

#endif // GLPAINTER_H
