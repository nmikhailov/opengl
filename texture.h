#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>

class Texture : public QObject {
    Q_OBJECT
public:
    Texture();
    virtual ~Texture();

    GLuint getId() const;
};

#endif // TEXTURE_H
