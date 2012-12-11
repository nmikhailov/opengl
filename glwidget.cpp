#include <sstream>
#include <iostream>

#include <Qt>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"

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
    //glEnable(GL_TEXTURE_2D);

    m_scene = new Scene(m_context);
    m_cam = new FreeLookCamera();
    m_scene->setRenderCamera(m_cam);
    m_scene->root()->add(new Axes());
    m_cam->setPosition(QVector3D(-5, 1, 1));
    m_cam->setViewVector(QVector3D(3, 0, 3));
    LightSource* light = new LightSource();
    //m_scene->root()->add(light);
    light->setPosition(QVector3D(-15, 1, 10));
    light->setDiffuseColor(Qt::white);
    light->setAttenuationType(QVector3D(0, 0.3, 0));

    light = new LightSource();
    light->setPosition(QVector3D(-5, 1, 3));
    light->setAttenuationType(QVector3D(0, 0.1, 0));
    m_l1 = light;

    m_scene->root()->add(light);

    AssimpModel *model = new AssimpModel("world.obj");
//    m_scene->root()->add(model);

    qDebug() << model->rect().xMin << " "
             << model->rect().xMax << " "
             << model->rect().yMin << " "
             << model->rect().yMax << " "
             << model->rect().zMin << " "
             << model->rect().zMax << " ";

    model->setPosition(QVector3D((model->rect().xMax + model->rect().xMin) / 2.,
                 (model->rect().yMax + model->rect().yMin) / 2.,
                 (model->rect().zMax + model->rect().zMin) / 2.));

    model->setPosition(QVector3D(57.8, 0, 4.4));
    qDebug() << model->position();
//    model->setScale(QVector3D(1. / (model->rect().xMax - model->rect().xMin),
//                 1. / (model->rect().yMax - model->rect().yMin),
//                 1. / (model->rect().zMax - model->rect().zMin)));


    m2 = new AssimpModel("airplane2.obj");
  //  m_scene->root()->add(m2);
    m2->setScale(QVector3D(1, 1, 1) * 1e-3);
    m2->setPosition(QVector3D(-3, 0.5, 3));

    m_l1->setPosition(QVector3D(0, 2, 3));
    m_l1->setDirection(m2->position() - m_l1->position());

    // Scene 2
    m2 = new AssimpModel("scene2.obj");
    //m2->setPosition(QVector3D(20, -15, 20));
    m2->setScale(QVector3D(1, 1, 1) * 1e-1);
    m_scene->root()->add(m2);
    m_time.start();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_cam->tick();
    m_scene->render();
    drawLegend();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_scene->setScreenSize(QVector2D(1024, 1024));
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

    if (event->key() == Qt::Key_Plus) {
        m_l1->setAttenuationType(m_l1->attenuation() * 0.9);
    } else if (event->key() == Qt::Key_Minus) {
        m_l1->setAttenuationType(m_l1->attenuation() * 1.1);
    } else if(event->key() == Qt::Key_Q) {
        m_rotation = !m_rotation;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    m_cam->keyReleaseEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event) {
}

void GLWidget::rotateOneStep() {
    static float x = 0;
    if (m_rotation)
        x += 0.5;
    m2->setRotation(QQuaternion(x, 0, 1, 0));
    static double y = m_l1->position().y();
    double ny = y + 3 * (sin(x / 100) + 1);
    //m_l1->setPosition(QVector3D(m_l1->position().x(), ny, m_l1->position().z()));
    //m_l1->setSpotAngle((son(x / 100) + 1) / 2. * 45);
    m_l1->setSpotAngle(45);

    QVector3D dir = m_l1->direction();
    dir.setX(sin(x / 300));
    dir.setZ(cos(x / 300));
    dir.normalize();
    //m_l1->setDirection(dir);
    static QVector3D base = m_l1->position();
    m_l1->setPosition(base + QVector3D(0, 1, 0) * cos(x / 300));
    m_l1->setDirection(m2->position() - m_l1->position());
    updateGL();
}
