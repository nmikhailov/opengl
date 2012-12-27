#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QObject>

#include "scene.h"

/*
 * Camera class
 */

class Camera : public QObject {
    Q_OBJECT
public:
    Camera();
    virtual ~Camera() {}

    virtual void setPosition(const QVector3D & pos);
    virtual QVector3D position() const;

    virtual QMatrix4x4 projectionMatrix() const = 0;
    virtual QMatrix4x4 viewMatrix() const = 0;

    // Manage screen size
    virtual void setScreenSize(const QSize &rect);
    virtual QSize screenSize() const;

protected:
    QVector3D m_pos;
    QSize m_screen_size;
};

#endif // CAMERA_H

