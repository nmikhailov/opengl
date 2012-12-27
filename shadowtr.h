#ifndef SHADOWTR_H
#define SHADOWTR_H

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

class ShadowTr : GLRenderer {
    Q_OBJECT
public:
    ShadowTr();
    ~ShadowTr();

    void setLight(LightSource *light, const QMatrix4x4 &model);
    void setShadowTexture(GLuint id);
    void setPreviousTexture(GLuint id);

    void setModelMatrix(const QMatrix4x4 &proj);
    void setViewMatrix(const QMatrix4x4 & proj);
    void setProjectionMatrix(const QMatrix4x4 & proj);

    void render(const GLObject* obj);

    bool bind();
    void release();

    // Ignoring
    void updateLight(const std::map<LightSource*, QMatrix4x4> &lights, const std::map<LightSource*, GLuint>  &lights_tex);
    void setMaterial(const Material &m);
protected:
    void setVertexBuffer(QGLBuffer buff, GLenum type = GL_FLOAT, int tupleSize = 3);
    void setIndexBuffer(QGLBuffer buff);

    void bindTexture(GLuint id);

    QMatrix4x4 m_M, m_P, m_V, m_L;
    QGLBuffer m_buff;
};

#endif // SHADOWTR_H
