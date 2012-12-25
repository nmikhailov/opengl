#ifndef FREELOOKCAMERA_H
#define FREELOOKCAMERA_H

#include <QVector3D>
#include <QTimer>
#include <functional>

#include "camera.h"

class FreeLookCamera : public Camera {
    Q_OBJECT
public:
    FreeLookCamera();
    virtual ~FreeLookCamera();

    virtual QVector3D direction() const;
    virtual void setViewVector(const QVector3D &view);

    virtual QVector3D normal() const;
    virtual void setNormal(const QVector3D &normal);

    QMatrix4x4 projectionMatrix() const;
    QMatrix4x4 viewMatrix() const;

public slots:
    virtual void mouseMoveEvent(QMouseEvent *event, QVector2D new_pos);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *);

    void tick();

protected:
    QVector3D m_normal, m_view;
    QVector2D m_last_mouse_pos;

    struct KeyFilter {
        bool enabled = false;

        QVector<Qt::Key> keys;
        QVector3D diff;
    };
    QVector<KeyFilter> m_keys;

    typedef std::function<void(KeyFilter & kf)> fn;
    void forEachFilter(fn f, int key);

    double m_speed = 0.005;
};

#endif // FREELOOKCAMERA_H
