#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QGLContext>
#include <QVector3D>
#include <QObject>
#include <QMatrix4x4>

#include "contextmanager.h"
#include "lightsource.h"
#include "scene.h"
#include "transformable.h"

/*
 * Basic drawable opengl object
 */

class GLObject : public Transformable {
    Q_OBJECT
public:
    friend class Scene;

    virtual void draw() const = 0;
signals:
    void transformationChanged(GLObject *sender);

protected:
    GLObject(Scene * scene);
    virtual ~GLObject();

    Scene * m_scene;
};

#endif // GLOBJECT_H
