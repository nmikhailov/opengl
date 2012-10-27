#include "group.h"

#include <algorithm>

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
        qDebug() << "Error. Trying to add canera from other scene to group";
    }
}

void Group::add(GLObject *obj) {
    if (std::find(m_scene->m_objects.begin(), m_scene->m_objects.end(), cam) != m_scene->m_objects.end()) {
        m_scene->remove(cam);
        m_cameras.push_back(cam);
    } else {
        qDebug() << "Error. Trying to add object from other scene to group";
    }
}

void Group::add(LightSource *light) {
}

void Group::remove(Camera *cam)
{
}

void Group::remove(GLObject *obj)
{
}

void Group::remove(LightSource *light)
{
}

int Group::getObjectCount() const
{
}

const GLObject *Group::getObject(int id) const
{
}

GLObject *Group::getObject(int id)
{
}

int Group::getLightCount() const
{
}

const LightSource *Group::getLight(int id) const
{
}

LightSource *Group::getLight(int id)
{
}

int Group::getCameraCount() const
{
}

const Camera *Group::getCamera(int id) const
{
}

Camera *Group::getCamera(int id)
{
}
