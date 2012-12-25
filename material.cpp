#include "material.h"

Material::Material() {
}

Material::Material(TextureInfo tex) {
    m_type = C_TEXTURE;
    m_texture = tex;
}

Material::Material(QColor color) {
    m_type = C_UNIFROM;
    m_color = color;
}

Material::~Material() {
}

Material::COLORING_TYPE Material::type() const {
    return m_type;
}

void Material::setType(Material::COLORING_TYPE type) {
    m_type = type;
}

QColor Material::color() const {
    return m_color;
}

void Material::setColor(const QColor &color) {
    m_color = color;
}

TextureInfo Material::texture() const {
    return m_texture;
}

void Material::setTexture(const TextureInfo &tex) {
    m_texture = tex;
}
