#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include <deque>

#include "objects/camera.h"
#include "objects/landscape.h"
#include "texturemanager.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
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
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *);

private:
    Camera *m_camera;

    std::deque<ColoringModel*> m_cmodels;
    std::deque<TerraGen*> m_generators;

    TextureManager *m_texman;
    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;

    QString m_status;

    Landscape* m_landscape;
};

#endif
