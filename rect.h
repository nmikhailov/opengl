#ifndef RECT_H
#define RECT_H

#include <QVector3D>
#include <QMatrix4x4>

class Rect {
public:
    Rect();
    Rect(const Rect &r);
    Rect(const QVector3D &m_min, const QVector3D &m_max);

    double maxX() const;
    double minX() const;

    double maxY() const;
    double minY() const;

    double maxZ() const;
    double minZ() const;

    QVector3D min() const;
    QVector3D max() const;

    void setMaxX(double a);
    void setMaxY(double a);
    void setMaxZ(double a);

    void setMinX(double a);
    void setMinY(double a);
    void setMinZ(double a);

    void setMin(const QVector3D &vec);
    void setMax(const QVector3D &vec);

    Rect merge(const QVector3D &vec) const;

    Rect merge(const Rect &r);

    const Rect& operator=(const Rect &r);
    Rect operator*(const QMatrix4x4 &mat) const;

    QVector3D center() const;
    QVector3D size() const;

private:
    QVector3D m_min, m_max;
};

qreal vec3max(const QVector3D &vec);

#endif // RECT_H
