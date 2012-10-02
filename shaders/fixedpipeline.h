#ifndef FIXEDPIPELINE_H
#define FIXEDPIPELINE_H

#include "shaderprogram.h"

class FixedPipeline : public ShaderProgram {
    Q_OBJECT
public:
    bool bind();
    void setPTMatrix(const QMatrix4x4 &proj);
private:
    friend class ShaderManager;
    FixedPipeline(QGLContext * context);
};

#endif // FIXEDPIPELINE_H
