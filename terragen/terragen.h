#ifndef TERRAGEN_H
#define TERRAGEN_H

#include <vector>

#include <QString>
#include <QObject>

class TerraGen : public QObject {
    Q_OBJECT
public:
    TerraGen();
    TerraGen(int width, int height);
    virtual ~TerraGen();

    typedef std::vector<std::vector<double> > TTerrain;

    virtual const TTerrain &get() const;
    virtual void gen();
    virtual QString name() const = 0;

    // Getters / setters
    virtual int width() const;
    virtual void setWidth(int w);

    virtual int height() const;
    virtual void setHeight(int h);

protected:
    void init(int w, int h);
    virtual void _gen() const = 0;

    void flatter_cross(int n = 1) const;
    void flatter_sqrt(int n = 2) const;

    void normalize() const;

    int m_width, m_height;
    mutable bool m_updated;
    mutable TTerrain m_terrain;

signals:
    void terrainChanged();
};

#endif // TERRAGEN_H
