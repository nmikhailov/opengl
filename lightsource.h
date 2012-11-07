#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QColor>
#include <QVector3D>

class LightSource : public QObject {
    Q_OBJECT
public:
    enum LIGHT_TYPE {L_DOT};


    LightSource();
    virtual ~LightSource();

    void setType(LIGHT_TYPE type);
    LIGHT_TYPE type() const;

    void setPosition(const QVector3D &pos);
    QVector3D position() const;

    QColor diffuseColor() const;
    void setDiffuseColor(const QColor &color);

    QVector3D attenuation() const;
    void setAttenuationType(const QVector3D &att);

protected:
    QVector3D m_position, m_attenuation;
    QColor m_diffuse_color, m_specular_color;

    LIGHT_TYPE m_type;

    QVector3D m_pos;
};

#endif // LIGHTSOURCE_H
