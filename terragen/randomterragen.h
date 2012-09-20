#ifndef RANDOMTERRAGEN_H
#define RANDOMTERRAGEN_H
#include "terragen.h"

class RandomTerraGen : public TerraGen
{
public:
    RandomTerraGen();
    RandomTerraGen(int w, int h);

    virtual ~RandomTerraGen();
    virtual QString name() const;

protected:
    virtual void _gen() const;
};

#endif // RANDOMTERRAGEN_H
