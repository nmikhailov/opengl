#include <sstream>
#include <iostream>
#include <cassert>

#include <Qt>
#include <QtGui>
#include <QtOpenGL>
#include <QImage>
#include <QPixmap>

#include "glwidget.h"
#include "skycuberenderer.h"

GLWidget::GLWidget(QGLContext* context, QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(context, parent, shareWidget) {
    setMouseTracking(true);
    m_clear_color = Qt::black;
    m_context = context;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(0); // 60 fps

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

GLWidget::~GLWidget() {
    delete m_scene;
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(200, 200);
}

void GLWidget::setClearColor(const QColor &color) {
    m_clear_color = color;
}

void GLWidget::initializeGL() {
    qglClearColor(m_clear_color);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Scene
    m_scene = new Scene(m_context);

    // Camera
    m_cam = new FreeLookCamera();
    m_cam->setPosition(QVector3D(2, 1, 0));
    m_cam->setViewVector(QVector3D(-2, -1, 0));
    m_scene->setRenderCamera(m_cam);

    // Axes (dont work)
    // m_scene->root()->add(new Axes());

    // Lights
    m_light1 = new LightSource();
    m_light1->setPosition(QVector3D(2, 1, 0));
    m_light1->setDirection(-m_light1->position());
    m_light1->setDiffuseColor(Qt::white);
    m_light1->setAttenuationType(QVector3D(1, 0, 0));
    m_scene->root()->add(m_light1);

    // Models
    // Landscape
    m_landscape = new AssimpModel("land1c.obj");
    //m_landscape = new AssimpModel("scene2.obj");

    //m_landscape->setPosition(-m_landscape->rect().center());
    m_landscape->setScale(QVector3D(1, 1, 1) / vec3max(m_landscape->rect().size()) * 2);

    m_scene->root()->add(m_landscape);

    // Skycube
    SkycubeRenderer *cr = new SkycubeRenderer(m_scene);

    m_box = new Skybox();
    m_box->setScale(QVector3D(1, 1, 1) * 1e2);
    m_box->setRenderer(cr);

    m_scene->root()->add(m_box);
}

void GLWidget::paintGL() {
    //
    m_box->setPosition(m_cam->position());

    m_cam->tick();
    m_scene->render();

    //assert(glGetError() == 0);
}

void GLWidget::resizeGL(int w, int h) {
    m_scene->setScreenSize(QSize(w, h));
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_last_mouse_pos = QVector2D(event->pos());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    QVector2D center(width() / 2, height() / 2);
    QPoint glob = mapToGlobal(QPoint(center.x(), center.y()));
    m_cam->mouseMoveEvent(event, center);
    QCursor::setPos(glob);
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */) {
    emit clicked();
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    m_cam->keyPressEvent(event);

    if (event->key() == Qt::Key_Q) {
        m_rotation = !m_rotation;
    } else if (event->key() == Qt::Key_F1) {
        m_shadow_sizes_id = (m_shadow_sizes_id + 1) % (sizeof(m_shadow_sizes) / sizeof(int));
        m_scene->setShadowMapSize(QSize(m_shadow_sizes[m_shadow_sizes_id], m_shadow_sizes[m_shadow_sizes_id]));
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    m_cam->keyReleaseEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent*) {
}

void GLWidget::rotateOneStep() {
    static double x = 0;
    if (m_rotation)
        x += 1e-1;


    //m_light1->setPosition(m_light1->position() + QVector3D(0, 0.001, 0));
    //m_light1->setDirection(-m_light1->position());
    m_landscape->setRotation(QQuaternion(x, 0, 1, 0));

    updateGL();
}
