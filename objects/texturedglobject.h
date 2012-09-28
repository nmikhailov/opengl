#ifndef TEXTUREDGLOBJECT_H
#define TEXTUREDGLOBJECT_H

#include "globject.h"

class TexturedGLObject : public GLObject {
    Q_OBJECT
public:
    TexturedGLObject(TextureManager * texman, MatrixStackManager * ms_manager) :
        GLObject(ms_manager) {
        m_texman = texman;
    }
    virtual ~TexturedGLObject() {}

protected:
    TextureManager * m_texman;
};

#endif // TEXTUREDGLOBJECT_H
