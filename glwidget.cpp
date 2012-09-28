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
    m_clear_color = Qt::black;

    m_sh_program = new QGLShaderProgram(context());
    m_texman = new TextureManager(this);
    m_msm = new MatrixStackManager(m_sh_program);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    m_cmodels.push_back(nullptr);
    m_cmodels.push_back(new HueColoringModel());
    m_cmodels.push_back(new HeightColoringModel());

    m_generators.push_back(new RandomTerraGen(256, 256));
    m_generators.push_back(new DiamondSquareGen(256, 256));
    m_generators.push_back(new SphereGen(256, 256));
}

GLWidget::~GLWidget() {
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
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    m_camera = new LookAtCamera(m_msm);
    m_camera->setPosition(QVector3D(0, 1.5, -1.5));

    m_landscape = new Landscape(m_texman, m_msm, m_generators.back());
    m_landscape->setColoringModel(m_cmodels.back());

    m_landscape->setTexturing(true);
    m_landscape->setScale(QVector3D(1, 0.25, 1));
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
    qglClearColor(m_clear_color);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_msm->clear();
    m_msm->setPerspective((double)width() / height());

    m_camera->apply();

    m_landscape->draw();

    renderText(0, 10, getStatus());
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_last_mouse_pos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - m_last_mouse_pos.x();
    int dy = event->y() - m_last_mouse_pos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_landscape->rotateBy(-0.5 * dy, 0, 0.5 * dx);
        updateGL();
    }
    m_last_mouse_pos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */) {
    emit clicked();
}

void GLWidget::nextColoring() {
    if(m_cmodels[0] == nullptr) {
        m_landscape->setColoring(false);
    } else {
        m_landscape->setColoring(true);
        m_landscape->setColoringModel(m_cmodels[0]);
    }

    m_cmodels.push_back(m_cmodels.front());
    m_cmodels.pop_front();
}

void GLWidget::nextTerraGen() {
    m_landscape->setGenerator(m_generators.front());

    m_generators.push_back(m_generators.front());
    m_generators.pop_front();
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    switch (event->key()) {
    case Qt::Key_T:
        m_landscape->setTexturing(!m_landscape->texturing());
        break;
    case Qt::Key_C:
        nextColoring();
        break;
    case Qt::Key_G:
        nextTerraGen();
        break;
    case Qt::Key_Escape:
        QApplication::exit();
        break;
    };

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
    m_landscape->rotateBy(0, 0.1, 0);
    updateGL();
}
