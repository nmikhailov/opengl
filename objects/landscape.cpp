#include <iostream>
#include <QtOpenGL>

#include "landscape.h"

Landscape::Landscape(QGLContext * context, TerraGen *generator, QVector2D res) : GLObject(context){
    m_generator = generator;

    m_is_cached = false;
    m_is_cached_colors = false;

    setResolution(res);
}

Landscape::~Landscape() {
    delete m_generator;
    delete m_cm;
    delete m_cache_vertex;
    delete m_cache_index;
    delete m_cache_colors;
}

bool Landscape::isColoringOn() const {
    return m_coloring;
}

const ColoringModel *Landscape::coloring() const {
    return m_cm;
}

void Landscape::disableColoring() {
    m_coloring = false;
    m_is_cached_colors = false;
}

void Landscape::enableColoring(ColoringModel *cm) {
    delete m_cm;

    m_cm = cm;
    m_coloring = true;
    m_is_cached_colors = false;
}

const TerraGen *Landscape::generator() const {
    return m_generator;
}

void Landscape::setGenerator(TerraGen *generator) {
    delete m_generator;

    m_generator = generator;
    m_is_cached = false;

    setResolution(resolution());
}

QVector2D Landscape::resolution() const {
    return m_resolution;
}

void Landscape::setResolution(const QVector2D &res) {
    if(m_resolution != res) {
        m_resolution = res;
        m_generator->setHeight(res.x());
        m_generator->setWidth(res.y());
        m_is_cached = false;
    }
}

void Landscape::regenerateTerrain() {
    m_generator->gen();
}

void Landscape::_draw() const {
    int width = m_generator->width(), height = m_generator->height();
    gen_vertex_index();
    gen_colors_index();

    // TEXTURES
    GLuint  tex = m_context->bindTexture(QPixmap(":/images/side1.png"), GL_TEXTURE_2D);

    //GLuint  tex = bindTexture(QPixmap(":/images/363_tile_Grass.jpg"), GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 600, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 600, 600, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glTexImage2D(GL_TEXTURE_2D, 0, 4, 10, 10, 0, GL_RGBA, , );

    GLfloat * flo = new GLfloat[width * height * 2];

    for (int i = 0, p = 0; i < height; i++) {
        for (int j = 0; j < width; j++, p += 2) {
            flo[p + 0] = (double) i / height;
            flo[p + 1] = (double) j / width;
        }
    }


    // TEXTURES

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, m_cache_vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, flo);

    if(m_coloring){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_DOUBLE, 0, m_cache_colors);
    }

    glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT,
                   m_cache_index);

    if(m_coloring)
        glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Landscape::gen_vertex_index() const {
    int width = m_generator->width(), height = m_generator->height();

    if(!m_is_cached)
        m_is_cached_colors = false;

    if(!m_is_cached) {
        delete m_cache_vertex;
        delete m_cache_index;

        m_cache_vertex = new GLdouble[width * height * 3];
        m_cache_index = new GLuint[(width - 1) * (height - 1) * 6];

        const TerraGen::TTerrain &terr = m_generator->get();

        for (int i = 0, p = 0; i < height; i++) {
            for (int j = 0; j < width; j++, p += 3) {
                double ex = (j - height / 2.) / height;
                double ey = (i - width / 2.) / width;

                m_cache_vertex[p + 0] = ey;
                m_cache_vertex[p + 1] = ex;
                m_cache_vertex[p + 2] = terr[i][j] - 0.5;
            }
        }


        for (int i = 0, p = 0; i < height - 1; i++) {
            for (int j = 0; j < width - 1; j++, p += 6) {
                m_cache_index[p + 0] = width* i + j + 1;
                m_cache_index[p + 1] = width * i + j;
                m_cache_index[p + 2] = width * i + j + width;

                m_cache_index[p + 3] = width * i + j + 1;
                m_cache_index[p + 4] = width * i + j + width;
                m_cache_index[p + 5] = width * i + j + width + 1;
            }
        }

        m_is_cached = true;
    }
}

void Landscape::gen_colors_index() const {
    int width = m_generator->width(), height = m_generator->height();

    if(!m_is_cached_colors && m_coloring) {
        delete m_cache_colors;
        m_cache_colors = new GLdouble[width * height * 3];

        ColoringModel &cm = *m_cm;
        const TerraGen::TTerrain &terr = m_generator->get();

        for (int i = 0, p = 0; i < height; i++) {
            for (int j = 0; j < width; j++, p += 3) {
                auto color = cm(terr[i][j]); // Z coord

                m_cache_colors[p + 0] = color.redF();
                m_cache_colors[p + 1] = color.greenF();
                m_cache_colors[p + 2] = color.blueF();
            }
        }

        m_is_cached_colors = true;
    }
}
