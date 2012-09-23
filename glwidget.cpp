#include <sstream>
#include <iostream>

#include <Qt>
#include <QtGui>
#include <QtOpenGL>

#include <GL/glu.h>

#include "glwidget.h"
#include "terragen/randomterragen.h"
#include "terragen/diamondsquaregen.h"
#include "terragen/spheregen.h"

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget) {
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    m_texman = new TextureManager(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    m_cmodels.push_back(nullptr);
    m_cmodels.push_back(new HueColoringModel());
    m_cmodels.push_back(new HeightColoringModel());
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
//    glEnable(GL_CULL_FACE);
     glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_camera = new Camera(QVector3D(0, -1.5, 1.5));

    m_landscape = new Landscape(m_texman, new SphereGen());
    m_landscape->setColoringModel(new HueColoringModel());
    //m_landscape->setColoring(true);
    m_landscape->setTexturing(true);
    m_landscape->setScale(QVector3D(1, 1, 0.25));

    //m_landscape = new Landscape(200, 200, new DiamondSquareGen(), QVector3D(1, 1, 2));
    // m_landscape->enableColoring(new HueColoringModel());
    //m_landscape->enableColoring(new HeightColorModel());
    //m_landscape->setPosition(QVector3D(0, 0, -0.5));
    // m_landscape->setScale(QVector3D(1.5, 1.5, 1));
}

QString GLWidget::getStatus() const {
    QString status = "";
    status += "Generator: " + m_landscape->generator()->name() + "; ";
    if(m_landscape->coloring())
        status += "Coloring model: " + m_landscape->coloringModel()->name() + "; ";
    else
        status += "Coloring off; ";
    if(m_landscape->texturing())
        status += "Textures on; ";
    else
        status += "Textures off; ";

    return status;
}

void GLWidget::paintGL() {
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->apply();
    m_status = getStatus();

    m_landscape->draw();

    renderText(0, 10, m_status);
}

void GLWidget::resizeGL(int width, int height) {
    int w = width, h = height;
    double ar = (double) w / (double) h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    m_camera->setAspectRatio(ar);
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

void GLWidget::keyPressEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_T) {
        m_landscape->setTexturing(!m_landscape->texturing());
    } else if(event->key() == Qt::Key_C) {
        if(m_cmodels[0] == nullptr) {
            m_landscape->setColoring(false);
        } else {
            m_landscape->setColoring(true);
            m_landscape->setColoringModel(m_cmodels[0]);
        }

        m_cmodels.push_back(m_cmodels.front());
        m_cmodels.pop_front();

    } else if(event->key() == Qt::Key_G) {
        //m_landscape->
    } else if(event->key() == Qt::Key_Escape) {
        QApplication::exit();
    }

    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    QVector3D lookFrom = m_camera->position();
    double d = lookFrom.y() + (event->delta() * 1e-3);
    lookFrom.setY(d);
    lookFrom.setZ(-d);
    m_camera->setPosition(lookFrom);
}

void GLWidget::rotateOneStep() {
    m_landscape->rotateBy(0, 0, 0.1);
    updateGL();
}
