#include "axis.h"

#include <QVector3D>


Axis::Axis(ContextManager *context):  GLObject(context) {
    QVector<GLfloat> vert;
    QVector<GLfloat> clr;
    QColor colors[] = {Qt::green, Qt::red, Qt::blue};
    for(int i = 0; i < 3; i++) {
        vert << 0 << 0 << 0;
        for(int j = 0; j < 3; j++) {
            if(i == j) {
                vert << 100;
            } else {
                vert << 0;
            }
        }
        for(int j = 0; j < 2; j++) {
            clr << colors[i].redF() << colors[i].greenF()  << colors[i].blueF();
        }
    }
    m_buff.create();
    m_buff.bind();
    m_buff.allocate(vert.constData(), vert.size() * sizeof(GLfloat));

    m_cl_buff.create();
    m_cl_buff.bind();
    m_cl_buff.allocate(clr.constData(), clr.size() * sizeof(GLfloat));
}

void Axis::draw() const {
    ColorShader sh = m_context->shaderManager()->setActiveShader<ColorShader>();
    sh.setColorMode(ColorShader::CM_COLOR_MAP);
    sh.setVertexBuffer(m_buff);
    sh.setColorBuffer(m_cl_buff);

    glLineWidth (2.0);
    glDrawArrays(GL_LINES, 0, m_buff.size() / sizeof(GLfloat));
}
