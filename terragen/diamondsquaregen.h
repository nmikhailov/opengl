#ifndef DIAMONDSQUAREGEN_H
#define DIAMONDSQUAREGEN_H

#include <QString>
#include "terragen.h"

class DiamondSquareGen : public TerraGen
{
public:
    virtual ~DiamondSquareGen();

    virtual QString name() const {
        return "Diamond square";
    }

protected:
    virtual void _gen() const;
    void fn(int l, int r, int t, int b, int iter) const;
};

#endif // DIAMONDSQUAREGEN_H
