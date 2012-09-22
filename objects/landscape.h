#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QVector2D>
#include <QtOpenGL>

#include "globject.h"
#include "coloring/coloringmodel.h"
#include "terragen/randomterragen.h"

class Landscape : public GLObject {
    Q_OBJECT
public:
    Landscape(TextureManager *context,  TerraGen * generator, QVector2D resolution = QVector2D(256, 256));
    virtual ~Landscape();

    virtual void regenerateTerrain();

    // Getters / setters
    virtual bool coloring() const;
    virtual void setColoring(bool val);

    virtual const ColoringModel * coloringModel() const;
    virtual void setColoringModel(ColoringModel * cm);

    virtual const TerraGen* generator() const;
    virtual void setGenerator(TerraGen * generator);

    virtual QVector2D resolution() const;
    virtual void setResolution(const QVector2D &res);

    virtual bool texturing() const;
    virtual void setTexturing(bool val);
private:
    TerraGen * m_generator = nullptr;
    ColoringModel * m_cm = nullptr;
    QVector2D m_resolution;

    bool m_coloring = false;
    bool m_texturing = false;


    mutable bool m_is_cached = false, m_is_cached_colors = false, m_is_cached_texture = false;

    mutable GLdouble * m_cache_vertex = nullptr;
    mutable GLuint * m_cache_index = nullptr;
    mutable GLdouble * m_cache_colors = nullptr;
    mutable GLdouble * m_cache_textures[3] = {nullptr, nullptr, nullptr};

    mutable GLuint m_cache_texid[3] = {0, 0, 0};
protected:
    virtual void _draw() const;

    virtual void genVertexIndex() const;
    virtual void genColorsIndex() const;
    virtual void genTextureIndex() const;

public slots:
    void terrainChanged();
};

#endif // LANDSCAPE_H
