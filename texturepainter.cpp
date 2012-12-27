#include "texturepainter.h"

namespace {
    const GLfloat screen[] = {
        -1, -1,
         1, -1,
        -1,  1,

        -1,  1,
         1, -1,
         1,  1,
    };
}

TexturePainter::TexturePainter() {
    m_shader.addShaderFromSourceFile(QGLShader::Fragment, ":/s/shaders/single_tex.frag");
    m_shader.addShaderFromSourceFile(QGLShader::Vertex, ":/s/shaders/single_tex.vert");
    m_shader.link();

    m_uv.create();
    m_uv.bind();
    m_uv.allocate(screen, sizeof(screen));
}

TexturePainter::~TexturePainter() {
}

void TexturePainter::bind() {
    m_shader.bind();
}

void TexturePainter::release() {
    m_shader.release();
}

void TexturePainter::renderTexture(GLuint texid) {
    m_uv.bind();
    m_shader.setAttributeBuffer("uv_coords", GL_FLOAT, 0, 2);
    m_shader.enableAttributeArray("uv_coords");


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);
    m_shader.setUniformValue("tex", 0);

   // glDrawArrays(GL_TRIANGLES, 0, 3);
}
