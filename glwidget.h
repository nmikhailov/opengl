#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <deque>

#include <QtGui>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QTime>

#include "camera/freelookcamera.h"
#include "texturemanager.h"
#include "objects/assimpmodel.h"
#include "objects/axes.h"
#include "objects/skybox.h"
#include "scene.h"

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget(QGLContext *context, QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

signals:
    void clicked();

private slots:
    void rotateOneStep();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void drawLegend();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *event);
private:
    QColor m_clear_color;
    QTime m_time;

    QVector2D m_last_mouse_pos; // For camera controlling

    // Context stuff
    QGLContext *m_context;

    // Scene objects
    Scene *m_scene;
    FreeLookCamera* m_cam;
    Group *m_landscape, *m_heli, *m_tree;
    Skybox *m_box;
    LightSource *m_light1, *m_light2;

    // Light source rotation
    bool m_rotation = true;
};

#endif
