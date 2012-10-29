#ifndef AXIS_H
#define AXIS_H

#include "globject.h"
#include "scene.h"

class Axis : public GLObject {
    Q_OBJECT
public:
    Axis(ContextManager * context);
    ~Axis() {}

protected:
    friend class Scene;
    void draw() const;
    QGLBuffer m_buff, m_cl_buff;
};

#endif // AXIS_H
