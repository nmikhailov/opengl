#include "rect.h"

#include <QDebug>
#include <limits>

Rect::Rect() {
    qreal inf = std::numeric_limits<qreal>::infinity();
    m_max = QVector3D(1, 1, 1) * -inf;
    m_min = QVector3D(1, 1, 1) * inf;
}

Rect::Rect(const Rect &r) {
    m_max = r.m_max;
    m_min = r.m_min;
}

Rect::Rect(const QVector3D &min, const QVector3D &max) {
    m_min = min;
    m_max = max;
}

double Rect::maxX() const {
    return m_max.x();
}

double Rect::minX() const {
    return m_min.x();
}

double Rect::maxY() const {
    return m_max.y();
}

double Rect::minY() const {
    return m_min.y();
}

double Rect::maxZ() const {
    return m_max.z();
}

double Rect::minZ() const {
    return m_min.z();
}

QVector3D Rect::min() const {
    return m_min;
}

QVector3D Rect::max() const {
    return m_max;
}

void Rect::setMaxX(double a) {
    m_max.setX(a);
}

void Rect::setMaxY(double a) {
    m_max.setY(a);
}

void Rect::setMaxZ(double a) {
    m_max.setZ(a);
}

void Rect::setMinX(double a) {
    m_min.setX(a);
}

void Rect::setMinY(double a) {
    m_min.setY(a);
}

void Rect::setMinZ(double a) {
    m_min.setZ(a);
}

void Rect::setMin(const QVector3D &vec) {
    m_min = vec;
}

void Rect::setMax(const QVector3D &vec) {
    m_max = vec;
}

Rect Rect::merge(const QVector3D &vec) const {
    Rect r;

    r.setMaxX(std::max(maxX(), vec.x()));
    r.setMinX(std::min(minX(), vec.x()));

    r.setMaxY(std::max(maxY(), vec.y()));
    r.setMinY(std::min(minY(), vec.y()));

    r.setMaxZ(std::max(maxZ(), vec.z()));
    r.setMinZ(std::min(minZ(), vec.z()));

    return r;
}

Rect Rect::merge(const Rect &r) {
    return this->merge(r.m_max).merge(r.m_min);
}

const Rect& Rect::operator =(const Rect &r) {
    m_min = r.min();
    m_max = r.max();
    return *this;
}

Rect Rect::operator *(const QMatrix4x4 &mat) const {
    return Rect(m_min * mat, m_max * mat);
}

QVector3D Rect::center() const {
    return (m_min + m_max) / 2.;
}

QVector3D Rect::size() const {
    return m_max - m_min;
}

qreal vec3max(const QVector3D &vec) {
    return std::max({vec.x(), vec.y(), vec.z()});
}
