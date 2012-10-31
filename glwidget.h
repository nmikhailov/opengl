#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <deque>

#include <QtGui>
#include <QGLWidget>
#include <QGLShaderProgram>

#include "camera/freelookcamera.h"
#include "texturemanager.h"
#include "objects/assimpmodel.h"
#include "objects/axes.h"
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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *event);
private:
    QColor m_clear_color;
    QVector2D m_last_mouse_pos;

    QString m_status; // Status text (top left screen corner)
    // Context stuff
    Scene *m_scene;
    QGLContext *m_context;
    FreeLookCamera* m_cam;
    AssimpModel * m2;
};

#endif
