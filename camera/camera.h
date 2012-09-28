#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QObject>

#include <matrixstackmanager.h>

/*
 * Camera class
 * apply() method should be called
 * BEFORE drawing any objects.
 */

class Camera : public QObject {
    Q_OBJECT
public:
    Camera(MatrixStackManager * msm);
    virtual ~Camera() {}
    
    virtual void setPosition(const QVector3D & pos);
    virtual QVector3D position() const;

    virtual void apply() const = 0;
protected:
    MatrixStackManager * m_msmanager;
    QVector3D m_pos;
};

#endif // CAMERA_H
