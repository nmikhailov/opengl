#include <iostream>

#include <QtOpenGL>

#include "landscape.h"

Landscape::Landscape(ContextManager * context, TerraGen *generator)
    : GLObject(context) {
    m_generator = generator;
    connect(m_generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));

    m_vertex_buffer_valid = false;
    m_color_buffer_valid = false;
    m_is_cached_texture = false;


    m_vertex_buffer.create();
    m_index_buffer = QGLBuffer(QGLBuffer::IndexBuffer);
    m_index_buffer.create();
    m_color_buffer.create();
}

Landscape::~Landscape() {
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
    m_color_buffer_valid = false;
}

const TerraGen *Landscape::generator() const {
    return m_generator;
}

void Landscape::setGenerator(TerraGen *generator) {
    disconnect(m_generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));
    connect(generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));

    m_generator = generator;
    m_vertex_buffer_valid = false;
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
    bool color = m_coloring, texture = false;

    updateVertexBuffer();
    updateColorBuffer();
    //genTextureIndex();

    if(color){
        m_color_buffer.bind();
        m_context->shaderProgram()->setAttributeBuffer(1, GL_FLOAT, 0, 3);
        m_context->shaderProgram()->enableAttributeArray(1);
    } else {
        m_context->shaderProgram()->disableAttributeArray(1);
    }

    m_vertex_buffer.bind();
    m_context->shaderProgram()->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_context->shaderProgram()->enableAttributeArray(0);

    m_index_buffer.bind();

    size_t sz = (width - 1) * (height - 1) * 6; // index array size
    glDrawElements(GL_TRIANGLES, sz, GL_UNSIGNED_INT, nullptr);
}

void Landscape::updateVertexBuffer() const {
    if (!m_vertex_buffer_valid) {
        m_color_buffer_valid = m_is_cached_texture = false;

        int width = m_generator->width(),
                height = m_generator->height();
        const TerraGen::TTerrain &terr = m_generator->get();

        QVector<GLfloat> vertexes;

        m_vertex_buffer.destroy();

        for (int i = 0, p = 0; i < height; i++) {
            for (int j = 0; j < width; j++, p += 3) {
                double ex = (j - height / 2.) / height;
                double ey = (i - width / 2.) / width;

                vertexes << ex << terr[i][j] - 0.5 << ey;
            }
        }
        m_vertex_buffer.bind();
        m_vertex_buffer.allocate(vertexes.constData(), sizeof(GLfloat) * vertexes.size());

        updateIndexBuffer();
        m_vertex_buffer_valid = true;
    }
}

void Landscape::updateIndexBuffer() const {
    if (!m_vertex_buffer_valid) {
        QVector<GLuint> indexes;
        m_color_buffer_valid = m_is_cached_texture = false;

        int width = m_generator->width(),
                height = m_generator->height();

        for (int i = 0; i < height - 1; i++) {
            for (int j = 0; j < width - 1; j++) {
                // Top
                indexes << width * i + j + 1
                        << width * i + j
                        << width * i + j + width;

                // Bottom
                indexes << width * i + j + 1
                        << width * i + j + width
                        << width * i + j + width + 1;
            }
        }

        m_index_buffer.bind();
        m_index_buffer.allocate(indexes.constData(), sizeof(GLuint) * indexes.size());
    }
}

void Landscape::updateColorBuffer() const {
    int width = m_generator->width(), height = m_generator->height();

    if(!m_color_buffer_valid && m_coloring) {
        QVector<GLfloat> colors;

        ColoringModel &cm = *m_cm;
        const TerraGen::TTerrain &terr = m_generator->get();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                auto color = cm(terr[i][j]); // Z coord

                colors << color.redF() << color.greenF() << color.blueF();
            }
        }
        m_color_buffer.bind();
        m_color_buffer.allocate(colors.constData(), sizeof(GLfloat) * colors.size());

        m_color_buffer_valid = true;
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
        m_cache_textures[tex_id] = new GLfloat[textures_size];

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
    m_vertex_buffer_valid = false;
}

