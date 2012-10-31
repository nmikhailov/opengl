#ifndef AXES_H
#define AXES_H

#include "globject.h"
#include "group.h"
#include "scene.h"

class Axes : public Group {
    Q_OBJECT
protected:
    friend class Scene;

    Axes(Scene *scene);
};

class Axis: public GLObject {
    Q_OBJECT
public:
    enum AXIS_TYPE {X, Y, Z};

    Material material() const;
    BufferInfo vertexBuffer() const;
    GLenum primitiveType() const;

    Rect rect() const;

protected:
    friend class Scene;

    Axis(Scene* scene, AXIS_TYPE type);
    Axis(Scene* scene, const QVector3D &vec, const QColor &color);

    void init(const QVector3D &vec, const QColor &color);

    Material m_mat;
    QGLBuffer m_buff;
};

#endif // AXES_H
