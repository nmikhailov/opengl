#include <iostream>

#include <QtOpenGL>
#include <GL/glu.h>

#include "landscape.h"

Landscape::Landscape(TextureManager * context, TerraGen *generator, QVector2D res) : GLObject(context){
    m_generator = generator;
    connect(m_generator, SIGNAL(terrainChanged()), this, SLOT(terrainChanged()));

    m_is_cached = false;
    m_is_cached_colors = false;
    m_is_cached_texture = false;

    setResolution(res);
}

Landscape::~Landscape() {
//    delete m_cm;
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
  //  delete m_cm;

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
    genVertexIndex();
    genColorsIndex();
    genTextureIndex();

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, m_cache_vertex);

    if(m_texturing) {
        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //glTexCoordPointer(2, GL_DOUBLE, 0, m_cache_textures[0]);

        //glBindTexture(GL_TEXTURE_2D, m_cache_texid[0]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////


        //glClientActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, m_cache_texid[0]);
        //glTexCoordPointer(2, GL_FLOAT, 0, m_cache_textures[0]);


        ///glClientActiveTextureARB(GL_TEXTURE1_ARB);
        //glTexCoordPointer(2, GL_FLOAT, 0, m_cache_textures[1]);
        //glBindTexture(GL_TEXTURE_2D, m_cache_texid[1]);

        //glEnableClientState(GL_TEXTURE_COORD_ARRAY);




///////////////////////////////////////////////////////////////////////////////////////////////////////////

        // TEX 1
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_cache_texid[0]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_DOUBLE, 0, m_cache_textures[0]);


        //glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
        //glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);


        // TEX 2
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_cache_texid[1]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_DOUBLE, 0, m_cache_textures[1]);



    }

    if(m_coloring){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_DOUBLE, 0, m_cache_colors);
    }
    glColor3f(1.0, 1.0, 1.0);
    glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT,
                   m_cache_index);

    if(m_coloring)
        glDisableClientState(GL_COLOR_ARRAY);

    if(m_texturing) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Landscape::genVertexIndex() const {
    int width = m_generator->width(), height = m_generator->height();

    if(!m_is_cached) {
        m_is_cached_colors = false;
        m_is_cached_texture = false;
    }

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

void Landscape::genColorsIndex() const {
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

void Landscape::genTextureIndex() const {
    int width = m_generator->width(), height = m_generator->height();

    if(!m_is_cached_texture && m_texturing) {
        delete m_cache_textures[0];
        delete m_cache_textures[1];
        delete m_cache_textures[2];

        // TEXTURES
        //QImage img = QPixmap(":/images/side1.png").toImage();
        QImage img = QImage(":/images/grass.png");
        for(int i = 0; i < img.width(); i++) {
            for(int j = 0; j < img.height(); j++) {
                QRgb color = img.pixel(i, j);

                int mi = i * ((double) m_generator->width() / img.width());
                int mj = j * ((double) m_generator->height() / img.height());

                double h = m_generator->get()[mi][mj];
                //color.setAlphaF(0.5);
                double cf = (1 - h);
                if(cf > 0.7)
                    cf = 1;
                else if(cf < 0.3)
                    cf = 0;
                color = qRgba(qRed(color), qGreen(color), qBlue(color), (int)(cf * 255));

                img.setPixel(i, j, color);
            }
        }


        m_cache_texid[0] = m_texman->loadTexture(img);

        img = QImage(":/images/rock.png");
        for(int i = 0; i < img.width(); i++) {
            for(int j = 0; j < img.height(); j++) {
                QRgb color = img.pixel(i, j);

                int mi = i * ((double) m_generator->width() / img.width());
                int mj = j * ((double) m_generator->height() / img.height());

                double h = m_generator->get()[mi][mj];
                //color.setAlphaF(0.5);
                double cf = h;

                color = qRgba(qRed(color), qGreen(color), qBlue(color), (int)(cf * 255));

                img.setPixel(i, j, color);
            }
        }
        m_cache_texid[1] = m_texman->loadTexture(img);
        //    glAlphaFunc(GL_EQUAL,   0.1f);
        //    glAlphaFunc(GL_LESS,	1.0f);

        //glBindTexture(GL_TEXTURE_2D, tex);


        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 600, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
        //gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 600, 600, GL_RGBA, GL_UNSIGNED_BYTE, tex);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        m_cache_textures[0] = new GLdouble[width * height * 2];

        for (int i = 0, p = 0; i < height; i++) {
            for (int j = 0; j < width; j++, p += 2) {
                m_cache_textures[0][p + 0] = (double) i / height;
                m_cache_textures[0][p + 1] = (double) j / width;
            }
        }

        m_cache_textures[1] = new GLdouble[width * height * 2];

        for (int i = 0, p = 0; i < height; i++) {
            for (int j = 0; j < width; j++, p += 2) {
                m_cache_textures[1][p + 0] = (double) i / height;
                m_cache_textures[1][p + 1] = (double) j / width;
            }
        }

        m_is_cached_texture = true;
    }
}

void Landscape::terrainChanged() {
    m_is_cached = false;
}

