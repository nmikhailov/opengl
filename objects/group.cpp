#include "group.h"

#include <algorithm>
#include <utility>
#include <QDebug>

Group::Group() : Transformable() {
}

Group::~Group() {
}

void Group::add(Camera *cam) {
    m_cameras.push_back(cam);
}

void Group::add(GLObject *obj) {
    m_objects.push_back(obj);
}

void Group::add(Group *obj) {
    m_groups.push_back(obj);
}

void Group::add(LightSource *light) {
    m_lights.push_back(light);
}

bool Group::remove(Camera *cam) {
    return remove(cam, m_cameras);
}

bool Group::remove(GLObject *obj) {
    return remove(obj, m_objects);
}

bool Group::remove(LightSource *light) {
    return remove(light, m_lights);
}

bool Group::remove(Group *g) {
    return remove(g, m_groups);
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

Rect Group::rect() const {
    Rect r;
    for (GLObject* obj: m_objects) {
        r = r.merge(obj->rect() * obj->modelMatrix());
    }
    return r;
}

QMatrix4x4 Group::modelMatrix() const {
    return Transformable::modelMatrix() /** m_base_transform*/;
}
