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

/*
 * OpenGL scene
 * Renders objects, lights, groups
 * Supports AA, complicated effects like mirroring, post effects
 */
class GLPainter;
class Group;

class Scene : public QObject {
    Q_OBJECT
public:
    Scene(QGLContext * context);
    virtual ~Scene();

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
    void renderToTexture(Texture * texture);

    // Update matrices map
    void updatePositions();


    // Initialize framebuffer object and texture
    void initFBO();

private:
    QGLContext *m_context;

    // Level 0 group
    Group * m_root;

    // Render target camera
    Camera *m_render_camera;

    // Object/light/group -> its postion
    std::map<Camera*, QMatrix4x4> m_cam_pos;
    std::map<LightSource*, QMatrix4x4> m_light_pos;
    std::map<GLObject*, QMatrix4x4> m_obj_pos;


    GLPainter *m_painter;
    TexturePainter *m_tex_painter;
    QVector2D m_screen_size;

    // Frambuffer
    //QGLFramebufferObject *m_fbo;
    GLuint m_fbo;
    GLuint m_btex[2];
    GLuint FramebufferName;
    GLuint renderedTexture, depthTexture;
};

#endif // SCENE_H

