#ifndef SPHEREGEN_H
#define SPHEREGEN_H

#include "terragen.h"

class SphereGen : public TerraGen {
    Q_OBJECT
public:
    SphereGen(int width=1, int height=1);
    virtual ~SphereGen() {}

    virtual QString name() const {
        return "Sphere";
    }

protected:
    virtual void _gen() const;
};

#endif // SPHEREGEN_H
