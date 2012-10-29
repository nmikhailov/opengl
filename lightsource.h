#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QObject>
#include <QVector3D>

class LightSource : public QObject {
    Q_OBJECT
public:

    void setPosition(const QVector3D &pos);
    QVector3D position() const;

protected:
    enum L_TYPE {L_1, L2};
    QVector3D m_position;
};

#endif // LIGHTSOURCE_H
