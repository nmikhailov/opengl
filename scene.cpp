#include "scene.h"
#include <queue>
#include <utility>

Scene::Scene(QGLContext *context) : m_context(context){
    m_root = newGroup<Group>();
    m_painter = new GLPainter(this);
}

Scene::~Scene() {
    delete m_root;
    delete m_painter;

    for (auto *obj: m_groups) {
        delete obj;
    }
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

int Scene::objectCount() const {
    return m_objects.size();
}

const GLObject *Scene::object(int id) const {
    return m_objects[id];
}

GLObject *Scene::object(int id) {
    return m_objects[id];
}

int Scene::lightCount() const {
    return m_lights.size();
}

const LightSource *Scene::light(int id) const {
    return m_lights[id];
}

LightSource *Scene::light(int id) {
    return m_lights[id];
}

int Scene::cameraCount() const {
    return m_cameras.size();
}

const Camera *Scene::camera(int id) const {
    return m_cameras[id];
}

Camera *Scene::camera(int id) {
    return m_cameras[id];
}

QMatrix4x4 Scene::modelMatrix(void *obj) const {
    auto it = m_positions.find(obj) ;
    if (it == m_positions.end()) {
        qDebug() << "Error. Requested model matrix is not caclulated";
        return QMatrix4x4();
    }
    return it->second;
    //return m_positions[obj];
}

bool Scene::inLastSceneTree(void *obj) const {
    return m_positions.find(obj) != m_positions.end();
}

void Scene::render() {
    // Texture *x = new Texture()
    // renderToTexture(x);
    // SM.setActiveShader(x)
    //
    updatePositions();


    m_painter->bind();
    m_painter->updateLight();
    m_render_camera->setScreenSize(m_screen_size);

    m_painter->setProjectionMatrix(m_render_camera->projectionMatrix());
    m_painter->setViewMatrix(m_render_camera->viewMatrix());
    for (GLObject *obj: m_objects) {
        if (!inLastSceneTree(obj))
            continue;

        QMatrix4x4 model = modelMatrix(obj) * obj->trMatrix();
        m_painter->setModelMatrix(model);

        m_painter->render(obj);
    }
}



void Scene::renderToTexture(Texture *texture) {
    // ?
}

void Scene::updatePositions() {
    m_positions.clear();

    std::queue<std::pair<Group*, QMatrix4x4> > q;
    q.push(std::make_pair(m_root, QMatrix4x4()));
    while(!q.empty()) {
        Group *cur = q.front().first;
        QMatrix4x4 model = q.front().second * cur->trMatrix();
        q.pop();

        // Groups
        for (int i = 0; i < cur->groupCount(); i++) {
            q.push(std::make_pair(cur->group(i), model));
        }

        // Objects
        for (int i = 0; i < cur->objectCount(); i++) {
            m_positions[cur->object(i)] = model;
        }

        // Lights
        for (int i = 0; i < cur->lightCount(); i++) {
            m_positions[cur->light(i)] = model;
        }

        // Cameras
        for (int i = 0; i < cur->cameraCount(); i++) {
            m_positions[cur->camera(i)] = model;
        }
    }

}

const Group *Scene::root() const {
    return m_root;
}

Group *Scene::root() {
    return m_root;
}

void Scene::setScreenSize(const QVector2D &rect) {
    m_screen_size = rect;
}

QVector2D Scene::screenSize() const {
    return m_screen_size;
}

Camera *Scene::renderCamera() {
    return m_render_camera;
}

const Camera *Scene::renderCamera() const {
    return m_render_camera;
}

void Scene::setRenderCamera(Camera *cam) {
    m_render_camera = cam;
}
