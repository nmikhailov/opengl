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
    virtual void add(Camera *cam);
    virtual void add(GLObject *obj);
    virtual void add(LightSource *light);

    // Remove Camera/Object/Light from scene
    virtual bool remove(Camera *cam);
    virtual bool remove(GLObject *obj);
    virtual bool remove(LightSource *light);

    // Deep remove Camera/Object/Light from scene
    virtual bool deepRemove(Camera *cam);
    virtual bool deepRemove(GLObject *obj);
    virtual bool deepRemove(LightSource *light);

    // Manage Objects
    virtual int getObjectCount() const;
    virtual const GLObject* getObject(int id) const;
    virtual GLObject* getObject(int id);

    // Manage Lights
    virtual int getLightCount() const;
    virtual const LightSource* getLight(int id) const;
    virtual LightSource* getLight(int id);

    // Manage Cameras
    virtual int getCameraCount() const;
    virtual const Camera* getCamera(int id) const;
    virtual Camera* getCamera(int id);

protected:
    void _draw() const;

    Group(Scene * scene);
    virtual ~Group();


protected:
    enum OBJ_TYPE{T_OBJECT, T_GROUP};

    std::vector<std::pair<GLObject*, OBJ_TYPE> > m_objects;
    std::vector<LightSource*> m_lights;
    std::vector<Camera*> m_cameras;
};

#endif // GROUP_H
