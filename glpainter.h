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
#include "glrenderer.h"

class GLObject;
class Scene;
class Material;

class GLPainter: public GLRenderer {
    Q_OBJECT
public:
    enum EColorMode {CM_ONE_COLOR = 0, CM_COLOR_MAP = 1, CM_TEXTURE = 2};

    void setModelMatrix(const QMatrix4x4 &proj);
    void setViewMatrix(const QMatrix4x4 & proj);
    void setProjectionMatrix(const QMatrix4x4 & proj);

    // New stuff
    void updateLight(const std::map<LightSource*, QMatrix4x4> &lights, const std::map<LightSource*, GLuint>  &lights_tex);
    void render(const GLObject* obj);
    void setMaterial(const Material &m);

    bool bind();
    void release();
protected:
    friend class Scene;

    // Used when one color mode is enabled
    void setColor(const QColor & color);
    void setColorMode(EColorMode mode);

    void setVertexBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setColorBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setNormalBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setIndexBuffer(QGLBuffer buff);

    void setUVBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 2);
    void bindTexture(GLuint id);

    GLPainter(Scene *scene);
    virtual ~GLPainter();

    Scene *m_scene;
};

#endif // GLPAINTER_H
