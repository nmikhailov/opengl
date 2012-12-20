#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QString>
#include <QtOpenGL>

class Texture {
public:
    QString m_name;
    int type = GL_TEXTURE_2D;
//    Texture() {};
//    virtual ~Texture() {};

    //GLuint getId() const;
};

#endif // TEXTURE_H
