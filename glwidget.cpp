#include <sstream>
#include <iostream>

#include <Qt>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "skycuberenderer.h"

GLWidget::GLWidget(QGLContext* context, QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(context, parent, shareWidget) {
    setMouseTracking(true);
    m_clear_color = Qt::blue;
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

    // Scene
    m_scene = new Scene(m_context);

    // Camera
    m_cam = new FreeLookCamera();
    m_cam->setPosition(QVector3D(0, 1, 0));
    m_cam->setViewVector(QVector3D(1, -1, 0));
    m_scene->setRenderCamera(m_cam);

    // Axes (dont work)
    m_scene->root()->add(new Axes());

    // Lights
    m_light1 = new LightSource();
    m_light1->setPosition(QVector3D(2, 1, 0));
    m_light1->setDirection(-m_light1->position());
    m_light1->setDiffuseColor(Qt::white);
    m_light1->setAttenuationType(QVector3D(0, 3, 0));
    m_scene->root()->add(m_light1);

    // Models
    // Landscape
    m_landscape = new AssimpModel("scene2.obj");
    m_landscape = new AssimpModel("land1c.obj");

    //m_landscape->setPosition(-m_landscape->rect().center());
    m_landscape->setScale(QVector3D(1, 1, 1) / vec3max(m_landscape->rect().size()) * 2);

    m_scene->root()->add(m_landscape);

    // Skycube
    SkycubeRenderer *cr = new SkycubeRenderer(m_scene);

    m_box = new Skybox();
    m_box->setScale(QVector3D(1, 1, 1) * 1e3);
    m_box->setRenderer(cr);

    m_scene->root()->add(m_box);
}

void GLWidget::paintGL() {
    //
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_box->setPosition(m_cam->position());

    m_cam->tick();
    m_scene->render();
    drawLegend();
}

void GLWidget::resizeGL(int w, int h) {
    m_scene->setScreenSize(QVector2D(w, h));
}

void GLWidget::drawLegend() {
    static int frames = 0;
    frames++;

    static double fps = 0;
    if (m_time.elapsed() > 1000) {
        fps = ((double) frames / m_time.elapsed()) * 1000.;
        m_time.start();
        frames = 0;
        qDebug() << "FPS: " << fps;
    }
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

    if(event->key() == Qt::Key_Q) {
        m_rotation = !m_rotation;
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
