#include "scene.h"
#include <queue>
#include <utility>

Scene::Scene(QGLContext *context) : m_context(context) {
    m_root = new Group();
    m_painter = new GLPainter(this);
    m_tex_painter = new TexturePainter();
}

Scene::~Scene() {
    delete m_root;
    delete m_painter;
    delete m_tex_painter;
}

void Scene::render() {
    // Texture *x = new Texture()
    // renderToTexture(x);
    // SM.setActiveShader(x)
    //
    updatePositions();
    //
//    GLuint FramebufferName = 0;
//    glGenFramebuffers(1, &FramebufferName);
//    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

//    // The texture we're going to render to
//    GLuint renderedTexture;
//    glGenTextures(1, &renderedTexture);

//    // "Bind" the newly created texture : all future texture functions will modify this texture
//    glBindTexture(GL_TEXTURE_2D, renderedTexture);

//    // Give an empty image to OpenGL ( the last "0" )
//    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

//    // Poor filtering. Needed !
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    //
//    // The depth buffer
//    GLuint depthrenderbuffer;
//    glGenRenderbuffers(1, &depthrenderbuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
//    //
//    // Set "renderedTexture" as our colour attachement #0
//    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

//    // Set the list of draw buffers.
//    GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0};
//    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    m_painter->bind();
    m_render_camera->setScreenSize(m_screen_size);

    m_painter->updateLight(m_light_pos);
    m_painter->setProjectionMatrix(m_render_camera->projectionMatrix());
    m_painter->setViewMatrix(m_render_camera->viewMatrix());
    for (auto &obj: m_obj_pos) {
        QMatrix4x4 model = obj.second * obj.first->trMatrix();
        m_painter->setModelMatrix(model);

        m_painter->render(obj.first);
    }
    m_painter->release();

    //

//    m_tex_painter->bind();
//    static int id = m_context->bindTexture(QImage(":/images/rock.png"));
//    m_tex_painter->renderTexture(id);
//    m_tex_painter->release();
}

void Scene::renderToTexture(Texture *texture) {
    // ?
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
