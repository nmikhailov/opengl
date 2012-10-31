#ifndef GROUP_H
#define GROUP_H

#include "globject.h"
#include "lightsource.h"
#include "camera/camera.h"
#include "scene.h"
#include "rect.h"
#include "transformable.h"

class Camera;
class GLObject;
class LightSource;

class Group : public Transformable {
    Q_OBJECT
public:
    friend class Scene;

    // Add Camera/Object/Light to scene
    // Checks scene ownership
    virtual void add(Camera *cam);
    virtual void add(GLObject *obj);
    virtual void add(Group *obj);
    virtual void add(LightSource *light);

    // Remove Camera/Object/Light from scene
    virtual bool remove(Camera *cam);
    virtual bool remove(GLObject *obj);
    virtual bool remove(LightSource *light);
    virtual bool remove(Group *g);

    // Deep remove Camera/Object/Light from scene
    virtual bool deepRemove(Camera *cam);
    virtual bool deepRemove(GLObject *obj);
    virtual bool deepRemove(LightSource *light);
    virtual bool deepRemove(Group *group);

    // Manage Objects
    virtual int objectCount() const;
    virtual const GLObject* object(int id) const;
    virtual GLObject* object(int id);

    // Manage Lights
    virtual int lightCount() const;
    virtual const LightSource* light(int id) const;
    virtual LightSource* light(int id);

    // Manage Cameras
    virtual int cameraCount() const;
    virtual const Camera* camera(int id) const;
    virtual Camera* camera(int id);

    // Manage subgroups
    virtual int groupCount() const;
    virtual const Group* group(int id) const;
    virtual Group* group(int id);

    Rect rect() const;

    //
    QMatrix4x4 trMatrix() const;
protected:
    Group(Scene * scene);
    virtual ~Group();


protected:
    enum OBJ_TYPE{T_OBJECT, T_GROUP};

    std::vector<GLObject*> m_objects;
    std::vector<Group*> m_groups;
    std::vector<LightSource*> m_lights;
    std::vector<Camera*> m_cameras;

    QMatrix4x4 m_base_transform;
    Scene *m_scene;
};

#endif // GROUP_H

