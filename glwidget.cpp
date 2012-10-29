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
    updateGL();
}

void GLWidget::initializeGL() {
    initContextManager((QGLContext*)context());
    qglClearColor(m_clear_color);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_BLEND);

    m_scene = new Scene(m_context);
    m_scene->add(m_scene->newCamera<FreeLookCamera>());
    //m_scene->add(m_scene->newCamera<FreeLookCamera>());
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_scene->render();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_last_mouse_pos = QVector2D(event->pos());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    QVector2D center(width() / 2, height() / 2);
    QPoint glob = mapToGlobal(QPoint(center.x(), center.y()));
    //m_camera->mouseMoveEvent(event, center);
    QCursor::setPos(glob);
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */) {
    emit clicked();
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    //m_camera->keyPressEvent(event);
    //updateGL();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    m_camera->keyReleaseEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event) {
}

void GLWidget::rotateOneStep() {
    //m_landscape->rotateBy(0, 0.1, 0);
    //m_landscape2->rotateBy(0, 0.1, 0);
    //m_plane2->rotateBy(0, 0.7, 0);
    //updateGL();
}
