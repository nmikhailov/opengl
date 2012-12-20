#include "axes.h"

#include <QVector3D>


Axes::Axes() {
    this->add(new Axis(Axis::X));
    this->add(new Axis(Axis::Y));
    this->add(new Axis(Axis::Z));
}

Axis::Axis(Axis::AXIS_TYPE type) {
    if (type == Axis::X) {
        init(QVector3D(1, 0, 0), Qt::red);
    } else if(type == Axis::Y) {
        init(QVector3D(0, 1, 0), Qt::green);
    } else {
        init(QVector3D(0, 0, 1), Qt::blue);
    }
}

Axis::Axis(const QVector3D &vec, const QColor &color) {
    init(vec, color);
}

void Axis::init(const QVector3D &vec, const QColor &color) {
    m_mat = Material(color);
    QVector3D v = vec.normalized();
    QVector<GLfloat> vt;
    vt << 0 << 0 << 0 << 0
       << v.x() << v.y() << v.z() << 0;

    m_buff.create();
    m_buff.bind();
    m_buff.allocate(vt.constData(), vt.size() * sizeof(GLfloat));
}


Material Axis::material() const {
    return m_mat;
}

GLObject::BufferInfo Axis::vertexBuffer() const {
    return BufferInfo(true, GL_FLOAT, 3, m_buff);
}

GLenum Axis::primitiveType() const {
    return GL_LINES;
}

Rect Axis::rect() const {
    Rect r(QVector3D(0, 0, 0), QVector3D(0, 0, 0));
    return r;
}
