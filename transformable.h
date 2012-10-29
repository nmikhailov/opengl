#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include <QObject>
#include <QMatrix4x4>

class Transformable : public QObject {
    Q_OBJECT
public:
    Transformable();
    virtual ~Transformable();

    virtual QVector3D position() const;
    virtual void setPosition(const QVector3D &vec);

    virtual QQuaternion rotation() const;
    virtual void setRotation(const QQuaternion &vec);

    virtual QVector3D scale() const;
    virtual void setScale(const QVector3D &vec);

    virtual QMatrix4x4 trMatrix() const;
protected:
    QVector3D m_position, m_scale;
    QQuaternion m_rotation;
};

#endif // TRANSFORMABLE_H
