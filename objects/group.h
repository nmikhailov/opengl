#ifndef GROUP_H
#define GROUP_H

#include "globject.h"
#include "lightsource.h"
#include "camera/camera.h"
#include "scene.h"

class Group : public GLObject {
    Q_OBJECT
public:
    friend class Scene;

    // Add Camera/Object/Light to scene
    // Checks scene ownership
    void add(Camera *cam);
    void add(GLObject *obj);
    void add(LightSource *light);

    // Remove Camera/Object/Light from scene
    void remove(Camera *cam);
    void remove(GLObject *obj);
    void remove(LightSource *light);

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

protected:
    void _draw() const;

    Group(Scene * scene);
    virtual ~Group();


private:
    std::vector<GLObject*> m_objects;
    std::vector<LightSource*> m_lights;
    std::vector<Camera*> m_cameras;
};

#endif // GROUP_H
