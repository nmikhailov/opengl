#include "scene.h"
#include "gldepthshader.h"

#include <queue>
#include <cassert>
#include <utility>

Scene::Scene(QGLContext *context) : m_context(context) {
    m_screen_size = QSize(100, 100);
    m_root = new Group();
    m_painter = new GLPainter(this);
    m_depth_painter = new GLDepthShader(this);
    m_tex_painter = new TexturePainter();
    m_texture_manager = new TextureManager(m_context);
    m_final = new TextureShader("single_tex.vert", "single_tex.frag");
    int sz = 512;
    m_shadow_map_size = QSize(sz, sz);
    //
    initFBO();
}

Scene::~Scene() {
    delete m_root;
    delete m_painter;
    delete m_tex_painter;
    delete m_depth_painter;

    delete m_main_fbo;
    for (int i = 0; i < max_lights; i++) {
        delete m_shadows[i];
    }
}

TextureManager *Scene::textureManager() {
    return m_texture_manager;
}

void Scene::render() {
    updatePositions();
    // Lights
    m_render_camera->setScreenSize(m_screen_size);
    renderLights();

    // Scene
    m_main_fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    GLRenderer *rnd = m_painter, *nr = m_painter;
    bindRenderer(rnd);

    for (auto &obj: m_obj_pos) {
        if (obj.first->renderer() != nullptr) {
            nr = obj.first->renderer();
        } else {
            nr = m_painter;
        }

        if(rnd != nr) {
            rnd = nr;
            bindRenderer(rnd);
        }

        QMatrix4x4 model = obj.second * obj.first->modelMatrix();
        rnd->setModelMatrix(model);

        rnd->render(obj.first);
    }
    rnd->release();

    // Final
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_screen_size.width(), m_screen_size.height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_final->render(m_main_fbo->textureBuffer());
}

void Scene::renderLights() {
    m_light_shadow.clear();
    int i = 0;
    for (auto rec = m_light_pos.begin(); rec != m_light_pos.end(); rec++, i++) {
        LightSource* light = rec->first;
        //
        m_shadows[i]->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderShadowMap(light);

        m_light_shadow[light] = m_shadows[i]->depthBuffer();
    }
}

void Scene::renderShadowMap(LightSource *light) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    m_depth_painter->bind();
    m_depth_painter->setProjectionMatrix(light->projectionMatrix(1));
    m_depth_painter->setViewMatrix(light->viewMatrix());

    for (auto &obj: m_obj_pos) {
        QMatrix4x4 model = obj.second * obj.first->modelMatrix();
        m_depth_painter->setModelMatrix(model);

        if (obj.first->renderer() == nullptr) {
            m_depth_painter->render(obj.first);
        }
    }
    m_depth_painter->release();
}

void Scene::updatePositions() {
    m_obj_pos.clear();
    m_cam_pos.clear();
    m_light_pos.clear();

    // bfs
    std::queue<std::pair<Group*, QMatrix4x4> > q;
    q.push(std::make_pair(m_root, QMatrix4x4()));

    while(!q.empty()) {
        Group *cur = q.front().first;
        QMatrix4x4 model = q.front().second * cur->modelMatrix();
        q.pop();

        // Groups
        for (int i = 0; i < cur->groupCount(); i++) {
            q.push(std::make_pair(cur->group(i), model));
        }

        // Objects
        for (int i = 0; i < cur->objectCount(); i++) {
            m_obj_pos[cur->object(i)] = model;
        }

        // Lights
        for (int i = 0; i < cur->lightCount(); i++) {
            m_light_pos[cur->light(i)] = model;
        }

        // Cameras
        for (int i = 0; i < cur->cameraCount(); i++) {
            m_cam_pos[cur->camera(i)] = model;
        }
    }

}

void Scene::bindRenderer(GLRenderer *renderer) {
    renderer->bind();

    renderer->updateLight(m_light_pos, m_light_shadow);
    renderer->setProjectionMatrix(m_render_camera->projectionMatrix());
    renderer->setViewMatrix(m_render_camera->viewMatrix());
}

void Scene::initFBO() {
    m_main_fbo = new Framebuffer(m_screen_size);

    for (int i = 0; i < max_lights; i++) {
        m_shadows[i] = new Framebuffer(m_shadow_map_size, false);
    }
}

const Group *Scene::root() const {
    return m_root;
}

Group *Scene::root() {
    return m_root;
}

void Scene::setScreenSize(const QSize &rect) {
    m_screen_size = rect;

    delete m_main_fbo;
    m_main_fbo = new Framebuffer(m_screen_size);
}

QSize Scene::screenSize() const {
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
