#ifndef SKYBOX_H
#define SKYBOX_H

#include <QGLBuffer>

#include "globject.h"


class Skybox : public GLObject {
public:
    Skybox();

    // Get objects material and vertex buffers
    Material material() const;
    BufferInfo vertexBuffer() const;
    GLenum primitiveType() const;

    // Get objects index, normal, tex buffers (optional)
    BufferInfo indexBuffer() const;
    //BufferInfo normalBuffer() const;
    //BufferInfo texcoordBuffer() const;

    // Object bounds
    Rect rect() const;

protected:
    void init();

    QGLBuffer m_buff, m_ind;
};

#endif // SKYBOX_H
