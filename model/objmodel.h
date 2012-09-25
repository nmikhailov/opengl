#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "model/model.h"

class TextureManager;
struct _GLMmodel;

class ObjModel : public Model {
    Q_OBJECT

public:
    explicit ObjModel(char* filename, TextureManager* context);
    ~ObjModel();

    virtual void draw() const;

protected:
    virtual void _draw() const;

private:
    _GLMmodel* m_model;
};

#endif // OBJMODEL_H
