#include <sstream>
#include <iostream>

#include <Qt>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"

GLWidget::GLWidget(QGLContext* context, QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(context, parent, shareWidget) {
    setMouseTracking(true);
    m_clear_color = Qt::black;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(1000 / 30); // 60 fps

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
    m_cam = m_scene->newCamera<FreeLookCamera>();
    m_scene->setRenderCamera(m_cam);
    m_scene->root()->add(m_scene->newGroup<Axes>());
    m_cam->setPosition(QVector3D(1, 1, 1));
    m_cam->setViewVector(QVector3D(-1, -1, -1));

    AssimpModel *model = m_scene->newGroup<AssimpModel>("world.obj");
    m_scene->root()->add(model);

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


    m2 = m_scene->newGroup<AssimpModel>("airplane2.obj");
    m_scene->root()->add(m2);
    m2->setScale(QVector3D(1, 1, 1) * 1e-3);
    m2->setPosition(QVector3D(-3, 0.5, 3));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_cam->tick();
    m_scene->render();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_scene->setScreenSize(QVector2D(w, h));
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
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    m_cam->keyReleaseEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event) {
}

void GLWidget::rotateOneStep() {
    static float x = 0;
    x += 0.1;
    //m2->setRotation(QQuaternion(x, 0, 1, 0));
    updateGL();
}
