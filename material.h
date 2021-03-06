#ifndef MATERIAL_H
#define MATERIAL_H

#include <QObject>
#include <vector>

#include "scene.h"
#include "texture.h"

class Material {
public:
    enum COLORING_TYPE {C_TEXTURE, C_UNIFROM};

    Material();
    Material(TextureInfo tex);
    Material(QColor color);
    ~Material();

    // get/set type
    COLORING_TYPE type() const;
    void setType(COLORING_TYPE type);

    // get/set color
    QColor color() const;
    void setColor(const QColor &color);

    // get/set texture
    TextureInfo texture() const;
    void setTexture(const TextureInfo &tex);

private:
    COLORING_TYPE m_type;
    QColor m_color;

    TextureInfo m_texture;
    // Add support for multiple textures
    //std::vector<Texture> m_textures;
};

#endif // MATERIAL_H
