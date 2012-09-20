#include <QtGui>
#include <QtOpenGL>
#include <sstream>
#include <GL/glu.h>
#include <QGLContext>
#include <iostream>

#include "glwidget.h"
#include "terragen/randomterragen.h"
#include "terragen/diamondsquaregen.h"

QVector3D lookFrom(0.0, -3.0, 3.0);
QVector3D transform(0, 0, 0);

const int width = 200;
const int width2 = width;

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget) {
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);
}

GLWidget::~GLWidget() {
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(200, 200);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle) {
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void GLWidget::setClearColor(const QColor &color) {
    clearColor = color;
    updateGL();
}

void GLWidget::initializeGL() {
    //makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);


    m_landscape = new Landscape(context(), new RandomTerraGen());
    context()->bindTexture();
    bindTexture();
    //m_landscape = new Landscape(200, 200, new DiamondSquareGen(), QVector3D(1, 1, 2));
    m_landscape->enableColoring(new HueColoringModel());
    //m_landscape->enableColoring(new HeightColorModel());
    //m_landscape->setPosition(QVector3D(0, 0, -0.5));
   // m_landscape->setScale(QVector3D(1.5, 1.5, 1));
}

QString GLWidget::getStatus() const {
    QString status = "";
    status += "Generator: " + m_landscape->generator()->name() + "; ";
    if(m_landscape->isColoringOn())
        status += "Coloring model: " + m_landscape->coloring()->name() + "; ";
    else
        status += "Coloring off; ";

    return status;
}

void GLWidget::paintGL() {
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, m_ar, 0.1f, 100000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, -1.5, 1.5, 0.0, 0.0, 0, 0.0, 1.0, 0.0);

    m_status = getStatus();

    m_landscape->draw();

    renderText(10, 10, m_status);
}

void GLWidget::resizeGL(int width, int height) {
    int w = width, h = height;
    m_ar = (double) w / (double) h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, m_ar, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_landscape->rotateBy(0.5 * dy, 0.5 * dx, 0);
        updateGL();
    }
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */) {
    emit clicked();
}

void GLWidget::keyPressEvent(QKeyEvent *) {

    updateGL();
}

void GLWidget::rotateOneStep() {
    m_landscape->rotateBy(0, 0, 0.1);
    updateGL();
}
