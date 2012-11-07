#ifndef AXES_H
#define AXES_H

#include "globject.h"
#include "group.h"

class Axes : public Group {
    Q_OBJECT
public:
    Axes();
};

class Axis: public GLObject {
    Q_OBJECT
public:
    enum AXIS_TYPE {X, Y, Z};

    Axis(AXIS_TYPE type);
    Axis(const QVector3D &vec, const QColor &color);

    Material material() const;
    BufferInfo vertexBuffer() const;
    GLenum primitiveType() const;

    Rect rect() const;

protected:
    void init(const QVector3D &vec, const QColor &color);

    Material m_mat;
    QGLBuffer m_buff;
};

#endif // AXES_H
