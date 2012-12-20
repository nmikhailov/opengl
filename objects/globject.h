#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLContext>
#include <QVector3D>
#include <QObject>
#include <QMatrix4x4>
#include <QGLBuffer>

#include "transformable.h"
#include "material.h"
#include "rect.h"
#include "glrenderer.h"

/*
 * Basic drawable opengl object
 */

class Material;

class GLObject : public Transformable {
    Q_OBJECT
public:
    GLObject();
    virtual ~GLObject();


    struct BufferInfo {
        BufferInfo();
        BufferInfo(bool enabled, GLenum type = 0, int sz = 0, QGLBuffer buff = QGLBuffer());

        QGLBuffer buff;
        GLenum type;

        bool enabled;
        int sz;
    };

    // Get objects material and vertex buffers
    virtual Material material() const = 0;
    virtual BufferInfo vertexBuffer() const = 0;
    virtual GLenum primitiveType() const = 0;

    // Get objects index, normal, tex buffers (optional)
    virtual BufferInfo indexBuffer() const;
    virtual BufferInfo normalBuffer() const;
    virtual BufferInfo texcoordBuffer() const;

    // Object bounds
    virtual Rect rect() const = 0;

    // Wanted render
    virtual GLRenderer* renderer() const;
    void setRenderer(GLRenderer* renderer);

protected:
    GLRenderer* m_renderer;
};

#endif // GLOBJECT_H
