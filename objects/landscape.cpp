#include <iostream>

#include <QtOpenGL>
#include <GL/glu.h>

#include "landscape.h"

Landscape::Landscape(ContextManager * context, TerraGen *generator)
    : GLObject(context) {
    m_generator = generator;
    connect(m_generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));

    m_is_vertexes_cached = false;
    m_is_triangles_cached = false;
    m_is_cached_colors = false;
    m_is_cached_texture = false;
}

Landscape::~Landscape() {
    delete m_cache_vertex;
    delete m_cache_index;
    delete m_cache_colors;
    for(int i = 0; i < 3; i++) {
        delete m_cache_textures[i];
    }
}

bool Landscape::coloring() const {
    return m_coloring;
}

const ColoringModel *Landscape::coloringModel() const {
    return m_cm;
}

void Landscape::setColoring(bool val) {
    m_coloring = val;
}

void Landscape::setColoringModel(ColoringModel *cm) {
    m_cm = cm;
    m_is_cached_colors = false;
}

const TerraGen *Landscape::generator() const {
    return m_generator;
}

void Landscape::setGenerator(TerraGen *generator) {
    disconnect(m_generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));
    connect(generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));

    m_generator = generator;
    m_is_vertexes_cached = false;
    m_is_triangles_cached = false;
}

bool Landscape::texturing() const {
    return m_texturing;
}

void Landscape::setTexturing(bool val) {
    if(m_texturing != val) {
        m_texturing = val;
        m_is_cached_texture = false;
    }
}

void Landscape::regenerateTerrain() {
    m_generator->gen();
}

void Landscape::_draw() const {
    int width = m_generator->width(), height = m_generator->height();
    bool color = false, texture = false;

    genVertexes();
    genTriangles();
    genColorsIndex();
    genTextureIndex();


    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, m_cache_vertex);

    if (texture) {
        for(size_t i = 0; i < m_texfiles.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m_cache_texid[i]);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
            glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_TEXTURE);
        }


        for(size_t i = 0; i < m_texfiles.size(); i++) {
            glClientActiveTexture(GL_TEXTURE0 + i);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_DOUBLE, 0, m_cache_textures[i]);
        }
    } else {
        for(size_t i = 0; i < m_texfiles.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    if(color){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_DOUBLE, 0, m_cache_colors);
    }

    glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT,
                   m_cache_index);

    if(color) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    if(texture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Landscape::genVertexes() const {
    if (!m_is_vertexes_cached) {
        m_is_cached_colors = m_is_cached_texture = false;

        int width = m_generator->width(),
                height = m_generator->height();
        const TerraGen::TTerrain &terr = m_generator->get();

        delete[] m_cache_vertex;
        m_cache_vertex = new GLdouble[width * height * 3];

        for (int i = 0, p = 0; i < height; i++) {
            for (int j = 0; j < width; j++, p += 3) {
                double ex = (j - height / 2.) / height;
                double ey = (i - width / 2.) / width;

                m_cache_vertex[p + 0] = ex;
                m_cache_vertex[p + 1] = terr[i][j] - 0.5;
                m_cache_vertex[p + 2] = ey;
            }
        }

        m_is_vertexes_cached = true;
    }
}

void Landscape::genTriangles() const {
    if (!m_is_triangles_cached) {
        m_is_cached_colors = m_is_cached_texture = false;

        int width = m_generator->width(),
                height = m_generator->height();

        delete[] m_cache_index;
        m_cache_index = new GLuint[(width - 1) * (height - 1) * 6];

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

        m_is_triangles_cached = true;
    }
}

void Landscape::genColorsIndex() const {
    int width = m_generator->width(), height = m_generator->height();

    if(!m_is_cached_colors && m_coloring) {
        delete[] m_cache_colors;
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

void Landscape::genTextureIndex() const {
    if (m_is_cached_texture) {
        return;
    }

    enum { Grass, Rock, Ice };
    TerraGen::TTerrain terrain = m_generator->get();
    int terrain_width = m_generator->width(),
        terrain_height = m_generator->height(),
        textures_size = terrain_width * terrain_height * 2;

    for(size_t tex_id = 0; tex_id < m_texfiles.size(); tex_id++) {
        QImage img(":/images/" + m_texfiles[tex_id]);
        int img_width = img.width(), img_height = img.height();
        double width_scale = ((double) terrain_width / img_width),
               height_scale = ((double) terrain_height / img_height);

        for(int i = 0; i < img_width; i++) {
            for(int j = 0; j < img_height; j++) {
                int mi = i * width_scale, mj = j * height_scale;
                double current_height = terrain[mi][mj], alpha = 0;

                switch (tex_id) {
                case Grass: alpha = current_height;					  break;
                case Rock:  alpha = (1 - current_height) * 0.8;       break;
                case Ice:   alpha = (current_height > 0.8) ? 0.3 : 1; break;
                };

                QColor color = img.pixel(i, j);
                color.setAlphaF(alpha);
                img.setPixel(i, j, color.rgba());
            }
        }

        m_cache_texid[tex_id] = m_context->textureManager()->loadTexture(img);

        delete[] m_cache_textures[tex_id];
        m_cache_textures[tex_id] = new GLdouble[textures_size];

        for (int i = 0, p = 0; i < terrain_height; i++) {
            for (int j = 0; j < terrain_width; j++, p += 2) {
                m_cache_textures[tex_id][p + 0] = (double) i / terrain_height;
                m_cache_textures[tex_id][p + 1] = (double) (terrain_width - j) / terrain_width;
            }
        }
    }

    m_is_cached_texture = true;
}

void Landscape::terrainChanged() {
    qDebug() << "changed";
    m_is_vertexes_cached = false;
    m_is_triangles_cached = false;
}

