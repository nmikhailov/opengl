#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGLContext>

#include <set>
#include <map>
#include <vector>

#include "objects/group.h"
#include "objects/globject.h"
#include "camera/camera.h"
#include "lightsource.h"
#include "texture.h"
#include "matrixstackmanager.h"

/*
 * OpenGL scene
 * Renders objects, lights, groups
 * Supports AA, complicated effects like mirroring, post effects
 */

class Scene : public QObject {
    Q_OBJECT
public:
    friend class Group;

    Scene(QGLContext * context);
    virtual ~Scene();


    template<class T>
    T* newObject() {
        static_assert(std::is_base_of<GLObject, T>::value,
                "Wrong type, should be GLObject subclass");

        m_objects.push_back(new T(this));
        return dynamic_cast<T*>(m_objects.back());
    }

    template<class T>
    T* newLightSource() {
        static_assert(std::is_base_of<LightSource, T>::value,
                "Wrong type, should be LightSource subclass");

        m_lights.push_back(new T(this));
        return dynamic_cast<T*>(m_lights.back());
    }

    template<class T>
    T* newCamera() {
        static_assert(std::is_base_of<LightSource, T>::value,
                "Wrong type, should be Camera subclass");

        m_cameras.push_back(new T(this));
        return dynamic_cast<T*>(m_cameras.back());
    }

    Group* newGroup();

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
    void renderCamera(Camera * cam);

    // Manage root group
    const Group* root() const;
    Group* root();

    // Render scene to screen
    void render();

    // Transformation matrix for object. Works on last scene postions update
    QMatrix4x4 modelMatrix (const void *obj) const;
    bool inLastSceneTree (const void *obj) const;

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
};

#endif // SCENE_H

