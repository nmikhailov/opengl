#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLContext>
#include <QVector3D>
#include <QObject>
#include <QMatrix4x4>

#include "contextmanager.h"

/*
 * Basic drawable opengl object
 */

class GLObject : public QObject {
    Q_OBJECT
public:
    GLObject(ContextManager * context);
    virtual ~GLObject();

    virtual void draw() const;

    // Getters / setters
    virtual QVector3D position() const;
    virtual void setPosition(const QVector3D &vec);

    virtual QMatrix4x4 rotation() const;
    virtual void setRotation(const QMatrix4x4 &mt);
    virtual void setRotation(const QVector3D &vec);

    virtual void rotateBy(double x, double y, double z);

    virtual QVector3D scale() const;
    virtual void setScale(const QVector3D &vec);

signals:
    void transformationChanged(GLObject *sender);

protected:
    QVector3D m_position, m_scale;
    QMatrix4x4 m_rotation;
    ContextManager * m_context;

    virtual void _draw() const = 0;
};

#endif // GLOBJECT_H
