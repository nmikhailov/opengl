#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGLContext>
#include <QVector2D>
#include <QGLFramebufferObject>

#include <set>
#include <map>
#include <vector>

#include "objects/group.h"
#include "objects/globject.h"
#include "camera/camera.h"
#include "lightsource.h"
#include "texture.h"
#include "glpainter.h"
#include "texturepainter.h"
#include "gldepthshader.h"
#include "texturemanager.h"

/*
 * OpenGL scene
 * Renders objects, lights, groups
 * Supports AA, complicated effects like mirroring, post effects
 */
class GLPainter;
class GLDepthShader;
class Group;

class Scene : public QObject {
    Q_OBJECT
public:
    Scene(QGLContext * context);
    virtual ~Scene();

    TextureManager* textureManager();
    // Manage Active camera(Render target)
    Camera* renderCamera();
    const Camera* renderCamera() const;
    void setRenderCamera(Camera * cam);

    // Manage root group
    const Group* root() const;
    Group* root();

    // Manage screen size
    void setScreenSize(const QVector2D &rect);
    QVector2D screenSize() const;

    // Render scene to screen
    void render();

protected:
    void renderLights();
    void renderShadowMap(LightSource* light);
    void renderTexture(GLuint tex_id);
    void renderTexture(GLuint tex_id, const QRect &rect);

    // Update matrices map
    void updatePositions();

    void bindRenderer(GLRenderer* renderer);


    // Initialize framebuffer object and texture
    void initFBO();

private:
    QGLContext *m_context;
    TextureManager *m_texture_manager;

    // Level 0 group
    Group * m_root;

    // Render target camera
    Camera *m_render_camera;

    // Object/light/group -> its postion
    std::map<Camera*, QMatrix4x4> m_cam_pos;
    std::map<LightSource*, QMatrix4x4> m_light_pos;
    std::map<LightSource*, GLuint> m_light_shadow;
    std::map<GLObject*, QMatrix4x4> m_obj_pos;


    // Shaders
    GLPainter *m_painter;
    TexturePainter *m_tex_painter;
    GLDepthShader *m_depth_painter;

    QVector2D m_screen_size;

    // Frambuffer
    //QGLFramebufferObject *m_fbo;
    GLuint m_fbo;
    GLuint m_btex[2];
    GLuint FramebufferName;
    GLuint renderedTexture, depthTexture;

    static const int max_lights = 20;
    GLuint m_depth_textures[max_lights];
    GLuint m_depth_framebuffers[max_lights];

    QGLFramebufferObject * fbo, *m_shadow_fbo;

    QSize m_shadow_map_size;
};

#endif // SCENE_H

