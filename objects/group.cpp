#include "group.h"

#include <algorithm>
#include <utility>

Group::Group(Scene *scene) : Transformable() {
    m_scene = scene;
}

Group::~Group() {
}

void Group::add(Camera *cam) {
    if (std::find(m_scene->m_cameras.begin(), m_scene->m_cameras.end(), cam) != m_scene->m_cameras.end()) {
        m_scene->root()->deepRemove(cam);
        m_cameras.push_back(cam);
    } else {
        qDebug() << "Error. Trying to add camera from other scene";
    }
}

void Group::add(GLObject *obj) {
    if (std::find(m_scene->m_objects.begin(), m_scene->m_objects.end(), obj) != m_scene->m_objects.end()) {
        m_scene->root()->deepRemove(obj);
        m_objects.push_back(obj);
    } else {
        qDebug() << "Error. Trying to add object from other scene";
    }
}

void Group::add(Group *obj) {
    if (std::find(m_scene->m_groups.begin(), m_scene->m_groups.end(), obj) != m_scene->m_groups.end()) {
        m_scene->root()->deepRemove(obj);
        m_groups.push_back(obj);
    } else {
        qDebug() << "Error. Trying to add group from other scene";
    }
}

void Group::add(LightSource *light) {
    if (std::find(m_scene->m_lights.begin(), m_scene->m_lights.end(), light) != m_scene->m_lights.end()) {
        m_scene->root()->deepRemove(light);
        m_lights.push_back(light);
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
    auto it = std::find(m_objects.begin(), m_objects.end(), obj);
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

bool Group::remove(Group *g) {
    auto it = std::find(m_groups.begin(), m_groups.end(), g);
    if (it != m_groups.end()) {
        m_groups.erase(it);
        return true;
    }
    return false;
}


bool Group::deepRemove(Camera *cam)  {
    if (remove(cam))
        return true;

    for (auto it = m_groups.begin(); it != m_groups.end(); it++) {
        if (it->deepRemove(cam))
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
    if (remove(light))
        return true;

    for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
        if (it->deepRemove(light))
            return true;
    }
    return false;
}

bool Group::deepRemove(Group *group) {
    if (remove(group))
        return true;

    for (auto it = m_groups.begin(); it != m_groups.end(); it++) {
        if (it->deepRemove(group))
            return true;
    }
    return false;
}

int Group::objectCount() const {
    return m_objects.size();
}

const GLObject *Group::object(int id) const {
    return m_objects[id];
}

GLObject *Group::object(int id) {
    return m_objects[id];
}

int Group::lightCount() const {
    return m_lights.size();
}

const LightSource *Group::light(int id) const {
    return m_lights[id];
}

LightSource *Group::light(int id) {
    return m_lights[id];
}

int Group::cameraCount() const {
    return m_cameras.size();
}

const Camera *Group::camera(int id) const {
    return m_cameras[id];
}

Camera *Group::camera(int id) {
    return m_cameras[id];
}

int Group::groupCount() const {
    return m_groups.size();
}

const Group *Group::group(int id) const {
    return m_groups[id];
}

Group *Group::group(int id) {
    return m_groups[id];
}
