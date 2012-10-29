#include "lightsource.h"


void LightSource::setPosition(const QVector3D &pos) {
    m_pos = pos;
}

QVector3D LightSource::position() const {
    return m_pos;
}

