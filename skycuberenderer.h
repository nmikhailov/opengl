#ifndef SKYCUBERENDERER_H
#define SKYCUBERENDERER_H

#include "glrenderer.h"

class Scene;

class SkycubeRenderer : public GLRenderer {
    Q_OBJECT
public:
    SkycubeRenderer(Scene* s);

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
    void init2();

    Scene *m_scene;
    GLuint m_tex_id;
    QMatrix4x4 m_model, m_view, m_proj;
};

#endif // SKYCUBERENDERER_H
