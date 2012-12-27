#include "scene.h"
#include "gldepthshader.h"
#include "shadowtr.h"

#include <QPainter>
#include <queue>
#include <cassert>
#include <utility>

Scene::Scene(QGLContext *context) : m_context(context) {
    m_screen_size = QSize(100, 100);
    m_root = new Group();
    m_painter = new GLPainter(this);
    m_depth_painter = new GLDepthShader(this);
    m_texture_manager = new TextureManager(m_context);
    m_final = new TextureShader("single_tex.vert", "single_tex.frag");

    // Shadows
    for (int i = 0; i < max_lights; i++) {
        m_shadows[i] = nullptr;
    }

    int sz = 64;
    m_shadow_map_size = QSize(sz, sz);
    //
    reinitFBO();
}

Scene::~Scene() {
    delete m_root;
    delete m_painter;
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
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    ShadowTr shdw;
    shdw.bind();
    shdw.setViewMatrix(m_render_camera->viewMatrix());
    shdw.setProjectionMatrix(m_render_camera->projectionMatrix());
    shdw.setViewMatrix(m_render_camera->viewMatrix());
    shdw.setLight(m_light_pos.begin()->first, m_light_pos.begin()->second);

    shdw.setShadowTexture(m_light_shadow.begin()->second);

    for (auto &obj: m_obj_pos) {
        if (obj.first->renderer() != nullptr) {
            continue;
        }

        QMatrix4x4 model = obj.second * obj.first->modelMatrix();
        shdw.setModelMatrix(model);
        shdw.render(obj.first);
    }
    shdw.release();
///
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_screen_size.width(), m_screen_size.height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
///
    GLRenderer *rnd = m_painter, *nr = m_painter;
    m_painter->setShadowMap(m_main_fbo->textureBuffer());
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
        m_painter->setShadowMap(m_main_fbo->textureBuffer());

        rnd->render(obj.first);
    }
    rnd->release();

    // Final
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glViewport(0, 0, m_screen_size.width(), m_screen_size.height());
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    m_final->render(m_main_fbo->textureBuffer());
    //
    if (m_debug)
        renderDebugInfo();
}

QSize Scene::shadowMapSize() const {
    return m_shadow_map_size;
}

void Scene::setShadowMapSize(const QSize &sz) {
    m_shadow_map_size = sz;
    reinitFBO();
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

        // Post process
    }
}

void Scene::renderShadowMap(LightSource *light) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    m_depth_painter->bind();
    m_depth_painter->setProjectionMatrix(light->projectionMatrix());
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

void Scene::reinitFBO() {
    delete m_main_fbo;
    m_main_fbo = new Framebuffer(m_screen_size);

    for (int i = 0; i < max_lights; i++) {
        delete m_shadows[i];
        m_shadows[i] = new Framebuffer(m_shadow_map_size, false);
    }
}

void Scene::renderDebugInfo() {
    static QTime time(0, 1);
    static int frames = 0;
    frames++;

    static double fps = 0;
    if (time.elapsed() > 1000) {
        fps = ((double) frames / time.elapsed()) * 1000.;
        time.start();
        frames = 0;
    }

    QSize size(m_screen_size.width(), 128);
    QRect rect(QPoint(0, 0), size);

    static QImage img(size, QImage::Format_RGB32);
    if (img.size() != m_screen_size) {
        img = QImage(size, QImage::Format_RGB32);
    }
    static QPainter painter;
    painter.begin(&img);
    painter.setBrush(Qt::SolidPattern);
    painter.setPen(Qt::black);
    painter.fillRect(rect, Qt::lightGray);

    QString text = "FPS: %1\nShadow map size: %2 x %3\nScreen size: %4 x %5";
    text = text.arg(fps);
    text = text.arg(m_shadow_map_size.width()).arg(m_shadow_map_size.height());
    text = text.arg(m_screen_size.width()).arg(m_screen_size.height());

    painter.drawText(rect, text);

    painter.end();

    glViewport(0, m_screen_size.height() - size.height(), size.width(), size.height());
    GLuint id = m_texture_manager->loadTexture(img, TextureInfo::T_2D);
    m_final->render(id);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &id);
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
