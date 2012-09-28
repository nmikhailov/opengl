#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QVector2D>
#include <QtOpenGL>

#include "globject.h"
#include "coloring/coloringmodel.h"
#include "terragen/randomterragen.h"

class Landscape : public GLObject {
    Q_OBJECT

    struct Geometry {
        GLdouble* vertexes;
        GLuint* triangles;

        Geometry(GLdouble* vs, GLuint* ts) : vertexes(vs), triangles(ts) {}
    };
public:
    Landscape(TextureManager *context,  MatrixStackManager * msm, TerraGen * generator);
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


    mutable bool m_is_vertexes_cached = false,
                 m_is_triangles_cached = false,
                 m_is_cached_colors = false,
                 m_is_cached_texture = false;

    mutable GLdouble * m_cache_vertex = nullptr;
    mutable GLuint * m_cache_index = nullptr;

    mutable GLdouble * m_cache_colors = nullptr;

    mutable GLdouble * m_cache_textures[3] = {nullptr, nullptr, nullptr};
    mutable GLuint m_cache_texid[3] = {0, 0, 0};

    std::vector<QString> m_texfiles = {"grass.png", "rock.png", "ice.png"};

protected:
    virtual void _draw() const;

    virtual GLdouble* genVertexes() const;
    virtual GLuint* genTriangles() const;
    virtual Geometry genGeometry() const;
    virtual GLdouble* genColorsIndex() const;
    virtual void genTextureIndex() const;

public slots:
    void terrainChanged();
};

#endif // LANDSCAPE_H
