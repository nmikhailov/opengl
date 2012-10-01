#include <iostream>

#include <QtOpenGL>

#include "landscape.h"
#include "shaders/colorshader.h"
#include "shaders/textureblendshader.h"

Landscape::Landscape(ContextManager * context, TerraGen *generator)
    : GLObject(context) {
    m_generator = generator;
    connect(m_generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));

    m_vertex_buffer_valid = false;
    m_color_buffer_valid = false;
    m_uv_buffer_valid = false;


    m_vertex_buffer.create();
    m_index_buffer = QGLBuffer(QGLBuffer::IndexBuffer);
    m_index_buffer.create();
    m_color_buffer.create();

    m_uv_buffer.create();
}

Landscape::~Landscape() {
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
        m_uv_buffer_valid = false;
    }
}

void Landscape::regenerateTerrain() {
    m_generator->gen();
}

void Landscape::_draw() const {
    int width = m_generator->width(), height = m_generator->height();
    bool color = m_coloring, textures_enabled = m_texturing;

    updateVertexBuffer();
    updateColorBuffer();
    updateUVBuffer();

    if(textures_enabled) {
        TextureBlendShader sh = m_context->shaderManager()->setActiveShader<TextureBlendShader>();
        sh.setIndexBuffer(m_index_buffer);
        sh.setVertexBuffer(m_vertex_buffer);
        sh.setUVBuffer(m_uv_buffer);

        for(size_t i = 0; i < m_texfiles.size(); i++) {
            sh.bindTexture(i, m_cache_texid[i]);
        }
        sh.setProp(0.5, 0.5);
    } else {
        auto sh = m_context->shaderManager()->setActiveShader<ColorShader>();
        if(color){
            sh.setColorMode(ColorShader::COLOR_MAP);
            sh.setColorBuffer(m_color_buffer);
        } else {
            sh.setColorMode(ColorShader::ONE_COLOR);
            sh.setColor(Qt::white);
        }

        sh.setIndexBuffer(m_index_buffer);
        sh.setVertexBuffer(m_vertex_buffer);
    }

    size_t sz = (width - 1) * (height - 1) * 6; // index array size
    glDrawElements(GL_TRIANGLES, sz, GL_UNSIGNED_INT, nullptr);
}

void Landscape::updateVertexBuffer() const {
    if (!m_vertex_buffer_valid) {
        m_color_buffer_valid = m_uv_buffer_valid = false;

        int width = m_generator->width(),
                height = m_generator->height();
        const TerraGen::TTerrain &terr = m_generator->get();

        QVector<GLfloat> vertexes;

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
        m_color_buffer_valid = m_uv_buffer_valid = false;

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

void Landscape::updateUVBuffer() const {
    if (!m_uv_buffer_valid) {
        int terrain_width = m_generator->width(),
                terrain_height = m_generator->height();

        for(size_t tex_id = 0; tex_id < m_texfiles.size(); tex_id++) {
            QImage img(":/images/" + m_texfiles[tex_id]);
            m_cache_texid[tex_id] = m_context->textureManager()->loadTexture(img);
        }

        QVector<GLfloat> uv_coords;
        for (int i = 0; i < terrain_height; i++) {
            for (int j = 0; j < terrain_width; j++) {
                double u = (double) i / (terrain_height / 2);
                double v = (double) (terrain_width - j - 1) / (terrain_width / 2);
                uv_coords << u << v;
            }
        }
        m_uv_buffer.bind();
        m_uv_buffer.allocate(uv_coords.constData(), sizeof(GLfloat) * uv_coords.size());

        m_uv_buffer_valid = true;
    }
}

void Landscape::terrainChanged() {
    m_vertex_buffer_valid = false;
}

