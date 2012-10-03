#ifndef AXIS_H
#define AXIS_H

#include "globject.h"

class Axis : public GLObject {
    Q_OBJECT
public:
    Axis(ContextManager * context);
    ~Axis() {};
protected:
    void _draw() const;
    QGLBuffer m_buff, m_cl_buff;
};

#endif // AXIS_H
