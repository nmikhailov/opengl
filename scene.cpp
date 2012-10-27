#include "scene.h"

Scene::Scene(QGLContext *context) : m_context(context){
    m_root = newGroup();
}

Scene::~Scene() {
    for (auto *obj: m_objects) {
        delete obj;
    }
    for (auto *obj: m_lights) {
        delete obj;
    }
    for (auto *obj: m_cameras) {
        delete obj;
    }
}

Group *Scene::newGroup() {
    return newObject<Group>();
}

void Scene::add(Camera *cam) {
    m_root->add(cam);
}

void Scene::add(GLObject *obj) {
    m_root->add(obj);
}

void Scene::add(LightSource *light) {
    m_root->add(light);
}

void Scene::remove(Camera *cam) {
}

void Scene::remove(GLObject *obj) {
}

void Scene::remove(LightSource *light) {
}

void Scene::dealloc(Camera *cam) {
}

void Scene::dealloc(GLObject *obj) {
}

