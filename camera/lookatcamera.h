#ifndef LOOKATCAMERA_H
#define LOOKATCAMERA_H

#include "camera.h"

/*
 * Implementation of deprecated gluLookAt() function
 */

class LookAtCamera: public Camera {
    Q_OBJECT
public:
    LookAtCamera(MatrixStackManager * msm);
    ~LookAtCamera() {}

    void loadDefaults();

    void setNormal(const QVector3D & normal);
    QVector3D normal() const;

    void setLookAt(const QVector3D & lookAt);
    QVector3D lookAt() const;

    void setAngle(double angle);
    double angle() const;

    void apply() const;

protected:
    QVector3D m_normal, m_look_at;
    double m_angle;
};

#endif // LOOKATCAMERA_H
