#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <deque>

#include <QtGui>
#include <QGLWidget>
#include <QGLShaderProgram>

#include "camera/lookatcamera.h"
#include "objects/landscape.h"
#include "texturemanager.h"
#include "contextmanager.h"

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget(QGLContext *context, QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

    QString getStatus() const;
signals:
    void clicked();

private slots:
    void rotateOneStep();

protected:
    void initContextManager(QGLContext * context);
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *);

    void nextColoring();
    void nextTerraGen();

    void loadShaders();

private:
    std::deque<ColoringModel*> m_cmodels; // For cycling trought coloring models
    std::deque<TerraGen*> m_generators; // For cycling trought landscape generators

    QColor m_clear_color;
    QPoint m_last_mouse_pos;

    QString m_status; // Status text (top left screen corner)

    Landscape * m_landscape;

    LookAtCamera * m_camera;

    // Context stuff
    ContextManager * m_context;
    TextureManager * m_texman;
    QGLShaderProgram * m_sh_program;
    MatrixStackManager * m_msm;

};

#endif
