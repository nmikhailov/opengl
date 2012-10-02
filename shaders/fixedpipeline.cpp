#include <QtOpenGL>

#include "fixedpipeline.h"

FixedPipeline::FixedPipeline(QGLContext * context) :
    ShaderProgram(context) {
}


bool FixedPipeline::bind() {
    glUseProgram(0);
    return true;
}

void FixedPipeline::setPTMatrix(const QMatrix4x4 &proj) {
}
