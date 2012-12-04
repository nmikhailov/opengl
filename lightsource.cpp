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

QVector3D LightSource::direction() const {
    return m_direction;
}

void LightSource::setDirection(const QVector3D &dir) {
    m_direction = dir;
}

float LightSource::spotAngle() const {
    return m_angle;
}

void LightSource::setSpotAngle(double angle) {
    m_angle = angle;
}

LightSource::LightSource() {
    m_attenuation = QVector3D(1, 0, 0);
    m_diffuse_color = Qt::white;
    m_position = QVector3D(0, 0, 0);
    m_angle = 30;
    m_direction = QVector3D(0, -1, 0);
}

LightSource::~LightSource() {
}
