#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    if (objectName().isEmpty())
        setObjectName("MainWindow");

    resize(800, 800);

    QGLFormat glFormat;
    glFormat.setVersion(3, 0);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    m_context = new QGLContext(glFormat);
    m_widget = new GLWidget(m_context, this);
    m_widget->setObjectName("centralWidget");
    setCentralWidget(m_widget);

    setWindowTitle("GL_HW2");

    QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow() {
    delete m_widget;
    //delete m_context;
}
