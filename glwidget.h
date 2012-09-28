#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <deque>

#include <QtGui>
#include <QGLWidget>
#include <QGLShaderProgram>

#include "camera/lookatcamera.h"
#include "objects/landscape.h"
#include "texturemanager.h"

class GLWidget : public QGLWidget {
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
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *);

    void nextColoring();
    void nextTerraGen();

private:
    LookAtCamera *m_camera;

    std::deque<ColoringModel*> m_cmodels;
    std::deque<TerraGen*> m_generators;

    TextureManager *m_texman;
    QColor m_clear_color;
    QPoint m_last_mouse_pos;

    QString m_status;

    Landscape* m_landscape;
    MatrixStackManager * m_msm;

    QGLShaderProgram * m_sh_program;
};

#endif
