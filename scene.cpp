#include "scene.h"
#include "gldepthshader.h"

#include <queue>
#include <utility>

Scene::Scene(QGLContext *context) : m_context(context) {
    m_root = new Group();
    m_painter = new GLPainter(this);
    m_depth_painter = new GLDepthShader(this);
    m_tex_painter = new TexturePainter();
    m_texture_manager = new TextureManager(m_context);
    //
    initFBO();
}

Scene::~Scene() {
    delete m_root;
    delete m_painter;
    delete m_tex_painter;
    delete m_depth_painter;
    //delete m_fbo;
}

void Scene::render() {
    // Texture *x = new Texture()
    // renderToTexture(x);
    // SM.setActiveShader(x)
    //
    updatePositions();
    //
    m_render_camera->setScreenSize(m_screen_size);

    // Draw shadowmap
    //m_shadow_fbo->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0, 0, m_screen_size.x(), m_screen_size.y()); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_depth_painter->bind();
    LightSource *ls = (*m_light_pos.begin()).first;
    renderShadowMap(ls);

    glCullFace(GL_BACK);
    // Draw scene
    //glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    fbo->bind();
    glViewport(0, 0, m_screen_size.x(), m_screen_size.y()); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_painter->bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    QMatrix4x4 mats(0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0);

    m_painter->m_program->setUniformValue("shadows[0]", 1);
    m_painter->m_program->setUniformValue("lights[0].PV_light", ls->projectionMatrix() * ls->viewMatrix());
    m_painter->m_program->setUniformValue("bias", mats);

    m_painter->updateLight(m_light_pos);
    m_painter->setProjectionMatrix(m_render_camera->projectionMatrix());
    m_painter->setViewMatrix(m_render_camera->viewMatrix());

  //  m_painter->setProjectionMatrix(ls->projectionMatrix());
   // m_painter->setViewMatrix(ls->viewMatrix());
    for (auto &obj: m_obj_pos) {
        QMatrix4x4 model = obj.second * obj.first->trMatrix();
        m_painter->setModelMatrix(model);

        m_painter->render(obj.first);
    }
    m_painter->release();

    // Draw textures
    int w = m_screen_size.x(), h = m_screen_size.y();
    renderTexture(fbo->texture(), QRect(0, 0, w / 2, h));
    renderTexture(depthTexture, QRect(w / 2, 0, w, h / 2));
    renderTexture(renderedTexture, QRect(w / 2, h / 2, w, h));
}

void Scene::renderToTexture(Texture *texture) {
}

void Scene::renderShadowMap(LightSource *light) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);
    m_depth_painter->bind();
    m_depth_painter->setProjectionMatrix(light->projectionMatrix());
    m_depth_painter->setViewMatrix(light->viewMatrix());

    for (auto &obj: m_obj_pos) {
        QMatrix4x4 model = obj.second * obj.first->trMatrix();
        m_depth_painter->setModelMatrix(model);

        m_depth_painter->render(obj.first);
    }
    m_depth_painter->release();
}

void Scene::renderTexture(GLuint tex_id, const QRect &rect) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(rect.x(), rect.y(), rect.width(), rect.height());

    m_tex_painter->bind();
    m_tex_painter->renderTexture(tex_id);
    m_tex_painter->release();
}

void Scene::renderTexture(GLuint tex_id) {
    renderTexture(tex_id, QRect(QPoint(0, 0), m_screen_size.toPoint()));
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
        QMatrix4x4 model = q.front().second * cur->trMatrix();
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

void Scene::initFBO() {
    fbo = new QGLFramebufferObject(1024, 1024, QGLFramebufferObject::Depth);
    //m_shadow_fbo = new QGLFramebufferObject(1024, 1024, QGLFramebufferObject::Depth);
    //m_fbo->bind();
    //fbo = new QGLFramebufferObject(1366, 768);
    //fbo->bind();
    // ---------------------------------------------
    // Render to Texture - specific code begins here
    // ---------------------------------------------
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    renderedTexture = m_texture_manager->genFBTexture(1024, 1024);
    depthTexture = m_texture_manager->genFBDepthTexture(1024, 1024);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Depth texture alternative :
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);
    //glDrawBuffers(0, 0);

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer isn't complete";
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
