#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLContext>
#include <QVector3D>

#include "texturemanager.h"

class GLObject
{
public:
    GLObject(TextureManager * context);
    virtual ~GLObject();

    virtual void draw() const;

    // Getters / setters
    virtual QVector3D position() const;
    virtual void setPosition(const QVector3D &vec);

    virtual QVector3D rotation() const;
    virtual void setRotation(const QVector3D &vec);
    virtual void rotateBy(double x, double y, double z);

    virtual QVector3D scale() const;
    virtual void setScale(const QVector3D &vec);

protected:
    QVector3D m_position, m_rotation, m_scale;
    TextureManager * m_texman;

    virtual void _draw() const = 0;
};

#endif // GLOBJECT_H
