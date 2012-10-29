#include "scene.h"

Scene::Scene(QGLContext *context) : m_context(context){
    m_root = newGroup();
}

Scene::~Scene() {
    delete m_root;

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

void Scene::dealloc(Camera *cam) {
    auto it = std::find(m_cameras.begin(), m_cameras.end(), cam);

    if (it != m_cameras.end()) {
        m_root->deepRemove(cam);
        m_cameras.erase(it);

        delete cam;
    } else {
        qDebug() << "Error. Trying to dealloc wrong object";
    }
}

void Scene::dealloc(GLObject *obj) {
    auto it = std::find(m_objects.begin(), m_objects.end(), obj);

    if (it != m_objects.end()) {
        m_root->deepRemove(obj);
        m_objects.erase(it);

        delete obj;
    } else {
        qDebug() << "Error. Trying to dealloc wrong cam";
    }
}

void Scene::dealloc(LightSource *light) {
    auto it = std::find(m_lights.begin(), m_lights.end(), light);

    if (it != m_lights.end()) {
        m_root->deepRemove(light);
        m_lights.erase(it);

        delete light;
    } else {
        qDebug() << "Error. Trying to dealloc wrong light";
    }
}



void Scene::render() {
    // Texture *x = new Texture()
    // renderToTexture(x);
    // SM.setActiveShader(x)
    //
}

void Scene::renderToTexture(Texture *texture) {
    // ?
}
