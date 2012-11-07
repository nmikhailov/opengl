#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>

#include "globject.h"
#include "scene.h"

class Sphere : public GLObject {
    Q_OBJECT
public:

protected:
    friend class Scene;
    Sphere();
};

#endif // SPHERE_H
