#ifndef RANDOMTERRAGEN_H
#define RANDOMTERRAGEN_H
#include "terragen.h"

class RandomTerraGen : public TerraGen
{
public:
    //RandomTerraGen();
    RandomTerraGen(int width=1, int height=1);

    virtual ~RandomTerraGen();
    virtual QString name() const;

protected:
    virtual void _gen() const;
};

#endif // RANDOMTERRAGEN_H
