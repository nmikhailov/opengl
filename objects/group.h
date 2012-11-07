#ifndef GROUP_H
#define GROUP_H

#include "globject.h"
#include "lightsource.h"
#include "camera/camera.h"

#include "rect.h"
#include "transformable.h"

class Camera;
class GLObject;
class LightSource;

class Group : public Transformable {
    Q_OBJECT
public:
    Group();
    virtual ~Group();

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
    template<class T>
    bool deepRemove(T* obj) {
        if (remove(obj))
            return true;

        for (auto it = m_groups.begin(); it != m_groups.end(); it++) {
            if ((*it)->deepRemove(obj))
                return true;
        }
        return false;
    }

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
    std::vector<GLObject*> m_objects;
    std::vector<Group*> m_groups;
    std::vector<LightSource*> m_lights;
    std::vector<Camera*> m_cameras;

    QMatrix4x4 m_base_transform;

private:
    template<class T>
    bool remove(T* obj, std::vector<T*> vec) {
        auto it = std::find(vec.begin(), vec.end(), obj);
        if (it != vec.end()) {
            vec.erase(it);
            return true;
        }
        return false;
    }
};

#endif // GROUP_H

