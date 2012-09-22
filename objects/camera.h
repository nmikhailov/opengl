#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>

class Camera : public QObject {
    Q_OBJECT
public:
    Camera(const QVector3D & pos = QVector3D(0, 0, 0),
           const QVector3D & lookAt = QVector3D(0, 0, 0),
           const QVector3D & normal = QVector3D(0, 0, 1),
           double angle = 45);

    void setPosition(const QVector3D & pos);
    QVector3D position() const;

    void setNormal(const QVector3D & normal);
    QVector3D normal() const;

    void setLookAt(const QVector3D & lookAt);
    QVector3D lookAt() const;

    void setAngle(double angle);
    double angle() const;

    void setAspectRatio(double ar);

    void apply() const;
protected:
    QVector3D m_pos, m_normal, m_lookAt;
    double m_angle, m_aspect;
};

#endif // CAMERA_H
