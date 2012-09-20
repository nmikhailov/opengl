#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QVector2D>
#include <QtOpenGL>

#include "globject.h"
#include "coloring/coloringmodel.h"
#include "terragen/randomterragen.h"

class Landscape : public GLObject {
public:
    Landscape(TextureManager *context,  TerraGen * generator, QVector2D resolution = QVector2D(256, 256));
    virtual ~Landscape();

    virtual void regenerateTerrain();

    // Getters / setters
    virtual bool isColoringOn() const;
    virtual const ColoringModel * coloring() const;
    virtual void disableColoring();
    virtual void enableColoring(ColoringModel * cm);

    virtual const TerraGen* generator() const;
    virtual void setGenerator(TerraGen * generator);

    virtual QVector2D resolution() const;
    virtual void setResolution(const QVector2D &res);

private:
    TerraGen * m_generator = nullptr;
    ColoringModel * m_cm = nullptr;
    QVector2D m_resolution;
    bool m_coloring = false;


    mutable bool m_is_cached = false, m_is_cached_colors = false;

    mutable GLdouble * m_cache_vertex = nullptr;
    mutable GLuint * m_cache_index = nullptr;
    mutable GLdouble * m_cache_colors = nullptr;

protected:
    virtual void _draw() const;

    virtual void gen_vertex_index() const;
    virtual void gen_colors_index() const;
};

#endif // LANDSCAPE_H
