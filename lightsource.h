#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QObject>
#include <QVector3D>
#include "scene.h"

class Scene;
class LightSource : public QObject {
    Q_OBJECT
public:
    enum LIGHT_TYPE {L_DOT};

    void setType(LIGHT_TYPE type);
    LIGHT_TYPE type() const;

    void setPosition(const QVector3D &pos);
    QVector3D position() const;

    QColor diffuseColor() const;
    void setDiffuseColor(const QColor &color);

    QVector3D attenuation() const;
    void setAttenuationType(const QVector3D &att);

protected:
    friend class Scene;

    LightSource(Scene* scene);
    virtual ~LightSource();

    QVector3D m_position, m_attenuation;
    QColor m_diffuse_color, m_specular_color;

    LIGHT_TYPE m_type;

    Scene *m_scene;
    QVector3D m_pos;
};

#endif // LIGHTSOURCE_H
