#ifndef MODEL_H
#define MODEL_H

#include "objects/globject.h"

class TextureManager;

class Model : public GLObject {
    Q_OBJECT

public:
    explicit Model(TextureManager* context);
    virtual ~Model();
};

#endif // MODEL_H
