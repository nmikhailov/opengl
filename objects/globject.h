#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLContext>
#include <QVector3D>
#include <QObject>
#include <QMatrix4x4>

#include "contextmanager.h"
#include "lightsource.h"
#include "scene.h"

/*
 * Basic drawable opengl object
 */

class GLObject : public QObject {
    Q_OBJECT
public:
    friend class Scene;

    virtual void draw() const;

    // Getters / setters
    virtual QVector3D position() const;
    virtual void setPosition(const QVector3D &vec);

    virtual QQuaternion rotation() const;
    virtual void setRotation(const QQuaternion &vec);

    virtual QVector3D scale() const;
    virtual void setScale(const QVector3D &vec);

signals:
    void transformationChanged(GLObject *sender);

protected:
    GLObject(Scene * scene);
    virtual ~GLObject();

    QVector3D m_position, m_scale;
    QQuaternion m_rotation;

    Scene * m_scene;

    virtual void _draw() const = 0;
};

#endif // GLOBJECT_H
