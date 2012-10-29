#include "group.h"

#include <algorithm>
#include <utility>

Group::Group(Scene *scene) : GLObject(scene) {
}

Group::~Group() {
}

void Group::_draw() const {
    for (GLObject *x: m_objects) {
        x->draw();
    }
}

void Group::add(Camera *cam) {
    if (std::find(m_scene->m_cameras.begin(), m_scene->m_cameras.end(), cam) != m_scene->m_cameras.end()) {
        m_scene->remove(cam);
        m_cameras.push_back(cam);
    } else {
        qDebug() << "Error. Trying to add camera from other scene";
    }
}

void Group::add(GLObject *obj) {
    if (std::find(m_scene->m_objects.begin(), m_scene->m_objects.end(), cam) != m_scene->m_objects.end()) {
        m_scene->deepRemove(obj);
        try {
            m_objects.push_back(std::make_pair(dynamic_cast<Group*>(obj), T_GROUP));
        } catch (...) {
            m_objects.push_back(std::make_pair(obj, T_OBJECT));
        }
    } else {
        qDebug() << "Error. Trying to add object from other scene";
    }
}

void Group::add(LightSource *light) {
    if (std::find(m_scene->m_lights.begin(), m_scene->m_lights.end(), cam) != m_scene->m_lights.end()) {
        m_scene->remove(obj);
        m_objects.push_back(obj);
    } else {
        qDebug() << "Error. Trying to add light from other scene";
    }
}

bool Group::remove(Camera *cam) {
    auto it = std::find(m_cameras.begin(), m_cameras.end(), cam);
    if (it != m_cameras.end()) {
        m_cameras.erase(it);
        return true;
    }
    return false;
}

bool Group::remove(GLObject *obj) {
    auto it = std::find_if(m_objects.begin(), m_objects.end(), [&](auto &x) {
        return x.first() == obj;
    });

    if (it != m_objects.end()) {
        m_objects.erase(it);
        return true;
    }
    return false;
}

bool Group::remove(LightSource *light) {
    auto it = std::find(m_lights.begin(), m_lights.end(), light);
    if (it != m_lights.end()) {
        m_lights.erase(it);
        return true;
    }
    return false;
}

bool Group::deepRemove(Camera *cam)  {
    if(remove(cam))
        return true;

    for (auto it = m_objects.begin(); it != m_objects.end(); it++) {
        if (it->second() != T_GROUP)
            continue;
        if(((Group*)it->second()).deepRemove(cam))
            return true;
    }
    return false;
}

bool Group::deepRemove(GLObject *obj) {
    if(remove(obj))
        return true;

    for (auto it = m_objects.begin(); it != m_objects.end(); it++) {
        if (it->second() != T_GROUP)
            continue;
        if(((Group*)it->second()).deepRemove(obj))
            return true;
    }
    return false;
}

bool Group::deepRemove(LightSource *light) {
    if(remove(light))
        return true;

    for (auto it = m_objects.begin(); it != m_objects.end(); it++) {
        if (it->second() != T_GROUP)
            continue;
        if(((Group*)it->second()).deepRemove(light))
            return true;
    }
    return false;
}

int Group::getObjectCount() const {
    return m_objects.size();
}

const GLObject *Group::getObject(int id) const {
    return m_objects[id];
}

GLObject *Group::getObject(int id) {
    return m_objects[id];
}

int Group::getLightCount() const {
    return m_lights.size();
}

const LightSource *Group::getLight(int id) const {
    return m_lights[id];
}

LightSource *Group::getLight(int id) {
    return m_lights[id];
}

int Group::getCameraCount() const {
    return m_cameras.size();
}

const Camera *Group::getCamera(int id) const {
    return m_cameras[id];
}

Camera *Group::getCamera(int id) {
    return m_cameras[id];
}
