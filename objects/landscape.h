#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QVector2D>
#include <QtOpenGL>
#include <QGLBuffer>

#include "globject.h"
#include "coloring/coloringmodel.h"
#include "terragen/randomterragen.h"

class Landscape : public GLObject {
    Q_OBJECT
public:
    Landscape(ContextManager *context, TerraGen * generator);
    virtual ~Landscape();

    virtual void regenerateTerrain();

    // Getters / setters
    virtual bool coloring() const;
    virtual void setColoring(bool val);

    virtual const ColoringModel * coloringModel() const;
    virtual void setColoringModel(ColoringModel * cm);

    virtual const TerraGen* generator() const;
    virtual void setGenerator(TerraGen * generator);

    virtual bool texturing() const;
    virtual void setTexturing(bool val);
private:
    TerraGen * m_generator = nullptr;
    ColoringModel * m_cm = nullptr;

    bool m_coloring = false;
    bool m_texturing = false;


    mutable bool m_vertex_buffer_valid = false,
                 m_color_buffer_valid = false,
                 m_is_cached_texture = false;

    mutable GLfloat * m_cache_textures[3] = {nullptr, nullptr, nullptr};
    mutable GLuint m_cache_texid[3] = {0, 0, 0};

    mutable QGLBuffer m_vertex_buffer, m_index_buffer, m_color_buffer;

    std::vector<QString> m_texfiles = {"grass.png", "rock.png", "ice.png"};


protected:
    virtual void _draw() const;

    virtual void updateVertexBuffer() const;
    virtual void updateIndexBuffer() const;
    virtual void updateColorBuffer() const;
    virtual void genTextureIndex() const;

public slots:
    void terrainChanged();
};

#endif // LANDSCAPE_H
