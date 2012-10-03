#include <sstream>
#include <iostream>

#include <Qt>
#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "terragen/randomterragen.h"
#include "terragen/diamondsquaregen.h"
#include "terragen/spheregen.h"

GLWidget::GLWidget(QGLContext* context, QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(context, parent, shareWidget) {
    m_clear_color = Qt::black;

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
    delete m_camera;
    delete m_landscape;
    delete m_landscape2;
    delete m_context;
    delete m_texman;
    delete m_shman;
    delete m_msm;
    delete m_plane1;
    for(auto x: m_cmodels)
        delete x;
    for(auto x: m_generators)
        delete x;
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
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);

    m_camera = new LookAtCamera(m_msm);
    m_camera->setPosition(QVector3D(0, 1.5, -1.5));

    m_landscape = new Landscape(m_context, m_generators.back());
    m_landscape->setColoringModel(m_cmodels.back());

    m_landscape->setTexturing(true);
    //m_landscape->setColoring(true);
    m_landscape->setScale(QVector3D(1, 0.25, 1));

    m_landscape2 = new Landscape(m_context, m_generators.back());
    m_landscape2->setColoringModel(m_cmodels.back());
    m_landscape2->setColoring(true);
    m_landscape2->setScale(QVector3D(0.25, 0.25 * 0.25, 0.25));
    m_landscape2->setPosition(QVector3D(0.5, 0.5, 0.5));

    m_plane1 = new AssimpModel(m_context);
    m_plane1->loadModel("airplane2b.obj");
    m_plane1->setPosition(QVector3D(1, 0.5, 0));

    m_axis = new Axis(m_context);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_msm->clear();
    m_msm->setPerspective((double)width() / height());
    m_camera->apply();

    m_axis->draw();

    m_landscape->draw();
    //m_landscape2->draw();
    static double x = 0;
    m_msm->push();
    m_msm->top().rotate(x, 0, 1, 0);
    x -= 0.5;
    m_msm->apply();
    m_plane1->draw();
    m_msm->pop();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_last_mouse_pos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - m_last_mouse_pos.x();
    int dy = event->y() - m_last_mouse_pos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_landscape->rotateBy(-0.5 * dy, 0, 0.5 * dx);
        //m_plane1->rotateBy(-0.5 * dy, 0, 0.5 * dx);
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
    m_landscape2->rotateBy(0, 0.1, 0);
    //m_plane1->rotateBy(0, 0.1, 0);
    updateGL();
}

void GLWidget::initContextManager(QGLContext *context) {
    m_context = new ContextManager(context);

    m_shman = new ShaderManager(context);
    m_texman = new TextureManager(context);
    m_msm = new MatrixStackManager(m_shman);

    m_context->setMatrixStackManager(m_msm);
    m_context->setTextureManager(m_texman);
    m_context->setShaderManager(m_shman);
}
