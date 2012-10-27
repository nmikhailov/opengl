#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGLContext>

#include "objects/group.h"
#include "objects/globject.h"
#include "camera/camera.h"
#include "lightsource.h"
#include "texture.h"

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

    // Add Camera/Object/Light to scene
    void add(Camera *cam);
    void add(GLObject *obj);
    void add(LightSource *light);

    // Remove Camera/Object/Light from scene
    void remove(Camera *cam);
    void remove(GLObject *obj);
    void remove(LightSource *light);

    // Remove and dealloc Camera/Object/Light from scene
    void dealloc(Camera *cam);
    void dealloc(GLObject *obj);
    void dealloc(LightSource *light);

    // Manage Objects
    int getObjectCount() const;
    const GLObject* getObject(int id) const;
    GLObject* getObject(int id);

    // Manage Lights
    int getLightCount() const;
    const LightSource* getLight(int id) const;
    LightSource* getLight(int id);

    // Manage Cameras
    int getCameraCount() const;
    const Camera* getCamera(int id) const;
    Camera* getCamera(int id);

    // Manage Active camera(Render target)
    Camera* getRenderCamera();
    const Camera* getRenderCamera() const;
    void setRenderCamera(Camera * cam);

    // Render scene to screen
    void render();
protected:
    void renderToTexture(Texture * texture);

private:
    // Level 0 group
    Group * m_root;

    // Allocated objects/lights/cameras
    std::vector<GLObject*> m_objects;
    std::vector<LightSource*> m_lights;
    std::vector<Camera*> m_cameras;

    // Render target camera
    Camera *m_render_camera;


    QGLContext * m_context;
};

#endif // SCENE_H
