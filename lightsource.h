#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>

class LightSource : public QObject {
    Q_OBJECT
public:
    LightSource();
    virtual ~LightSource();

    void setPosition(const QVector3D &pos);
    QVector3D position() const;

    QColor diffuseColor() const;
    void setDiffuseColor(const QColor &color);

    QVector3D attenuation() const;
    void setAttenuationType(const QVector3D &att);

    QVector3D direction() const;
    void setDirection(const QVector3D &dir);

    float spotAngle() const;
    void setSpotAngle(double angle);

    QMatrix4x4 projectionMatrix(double ar) const;
    QMatrix4x4 viewMatrix() const;

protected:
    QVector3D m_position, m_attenuation;
    QColor m_diffuse_color, m_specular_color;

    QVector3D m_direction;
    double m_angle;
};

#endif // LIGHTSOURCE_H
