#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QObject>
#include <QMatrix4x4>
#include <QtOpenGL>

#include <memory>

class LightSource;
class GLObject;
class Material;

class GLRenderer : public QObject {
    Q_OBJECT
public:
    GLRenderer();
    virtual ~GLRenderer();

    virtual void setModelMatrix(const QMatrix4x4 &proj) = 0;
    virtual void setViewMatrix(const QMatrix4x4 & proj) = 0;
    virtual void setProjectionMatrix(const QMatrix4x4 & proj) = 0;

    // New stuff
    virtual void updateLight(const std::map<LightSource*, QMatrix4x4> &lights, const std::map<LightSource*, GLuint>  &lights_tex) = 0;
    virtual void render(const GLObject* obj) = 0;
    virtual void setMaterial(const Material &m) = 0;

    virtual bool bind() = 0;
    virtual void release() = 0;

protected:
    void init(QString vert, QString frag);

    std::shared_ptr<QGLShaderProgram> m_program;
};

#endif // GLRENDERER_H
