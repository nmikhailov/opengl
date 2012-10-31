#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGLContext>
#include <QVector2D>

#include <set>
#include <map>
#include <vector>

#include "objects/group.h"
#include "objects/globject.h"
#include "camera/camera.h"
#include "lightsource.h"
#include "texture.h"
#include "glpainter.h"

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
    friend class Group;

    Scene(QGLContext * context);
    virtual ~Scene();


    template<typename T, typename... Args>
    T* newObject(Args... args) {
        static_assert(std::is_base_of<GLObject, T>::value,
                "Wrong type, should be GLObject subclass");

        m_objects.push_back(new T(this, args...));
        return (T*)m_objects.back();
    }

    template<typename T, typename... Args>
    T* newLightSource(Args... args) {
        static_assert(std::is_base_of<LightSource, T>::value,
                "Wrong type, should be LightSource subclass");

        m_lights.push_back(new T(this, args...));
        return (T*)m_lights.back();
    }

    template<typename T, typename... Args>
    T* newCamera(Args... args) {
        static_assert(std::is_base_of<Camera, T>::value,
                "Wrong type, should be Camera subclass");

        m_cameras.push_back(new T(this, args...));
        return (T*)m_cameras.back();
    }

    template<typename T, typename... Args>
    T* newGroup(Args... args) {
        static_assert(std::is_base_of<Group, T>::value,
                "Wrong type, should be Group subclass");

        m_groups.push_back(new T(this, args...));
        return (T*)m_groups.back();
    }

    // Remove and dealloc Camera/Object/Light from scene
    void dealloc(Camera *cam);
    void dealloc(GLObject *obj);
    void dealloc(LightSource *light);

    // Manage Objects
    int objectCount() const;
    const GLObject* object(int id) const;
    GLObject* object(int id);

    // Manage Lights
    int lightCount() const;
    const LightSource* light(int id) const;
    LightSource* light(int id);

    // Manage Cameras
    int cameraCount() const;
    const Camera* camera(int id) const;
    Camera* camera(int id);

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

    // Transformation matrix for object. Works on last scene postions update
    QMatrix4x4 modelMatrix (void *obj) const;
    bool inLastSceneTree (void *obj) const;

protected:
    void renderToTexture(Texture * texture);

    // Update matrices map
    void updatePositions();
private:
    // Level 0 group
    Group * m_root;

    // Allocated objects/lights/cameras
    std::vector<GLObject*> m_objects;
    std::vector<LightSource*> m_lights;
    std::vector<Camera*> m_cameras;
    std::vector<Group*> m_groups;

    // Render target camera
    Camera *m_render_camera;

    QGLContext *m_context;

    // Object/light/group -> its postion
    std::map<void*, QMatrix4x4> m_positions;

    GLPainter *m_painter;
    QVector2D m_screen_size;
};

#endif // SCENE_H

