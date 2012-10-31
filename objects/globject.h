#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLContext>
#include <QVector3D>
#include <QObject>
#include <QMatrix4x4>
#include <QGLBuffer>

#include "scene.h"
#include "transformable.h"
#include "material.h"
#include "rect.h"

/*
 * Basic drawable opengl object
 */

class Scene;
class Material;

class GLObject : public Transformable {
    Q_OBJECT
public:
    friend class Scene;

    struct BufferInfo {
        BufferInfo();
        BufferInfo(bool enabled, GLenum type = 0, int sz = 0, QGLBuffer buff = QGLBuffer());

        QGLBuffer buff;
        bool enabled;
        int sz;
        GLenum type;
    };

    // Get objects material and vertex buffers
    virtual Material material() const = 0;
    virtual BufferInfo vertexBuffer() const = 0;
    virtual GLenum primitiveType() const = 0;

    // Get objects index, normal, tex buffers (can be not present)
    virtual BufferInfo indexBuffer() const;
    virtual BufferInfo normalBuffer() const;
    virtual BufferInfo texcoordBuffer() const;

    virtual Rect rect() const = 0;

protected:
    GLObject(Scene* scene);
    virtual ~GLObject();

    Scene* m_scene;
};

#endif // GLOBJECT_H
