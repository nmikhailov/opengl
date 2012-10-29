#include "lightsource.h"


void LightSource::setPosition(const QVector3D &pos) {
    m_pos = pos;
}

QVector3D LightSource::position() const {
    return m_pos;
}

QColor LightSource::diffuseColor() const {
    return m_diffuse_color;
}

void LightSource::setDiffuseColor(const QColor &color) {
    m_diffuse_color = color;
}

QVector3D LightSource::attenuation() const {
    return m_attenuation;
}

void LightSource::setAttenuationType(const QVector3D &att) {
    m_attenuation = att;
}

LightSource::LightSource(Scene *scene) {
    m_scene = scene;
    m_attenuation = QVector3D(1, 0, 0);
    m_diffuse_color = Qt::white;
    m_position = QVector3D(0, 0, 0);
}

void LightSource::setType(LightSource::LIGHT_TYPE type) {
    m_type = type;
}

LightSource::LIGHT_TYPE LightSource::type() const {
    return m_type;
}
