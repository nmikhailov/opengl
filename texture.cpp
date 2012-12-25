#include "texture.h"

TextureInfo::TextureInfo() {
    m_type = T_2D;
}

TextureInfo::TextureInfo(const QString &file_name, TextureInfo::T_TYPE type) {
    m_name = file_name;
    m_type = type;
}

QString TextureInfo::fileName() const {
    return m_name;
}

void TextureInfo::setFileName(const QString &file_name) {
    m_name = file_name;
}

TextureInfo::T_TYPE TextureInfo::type() const {
    return m_type;
}

void TextureInfo::setType(TextureInfo::T_TYPE type) {
    m_type = type;
}

bool TextureInfo::operator ==(const TextureInfo &info) const {
    return m_name == info.fileName() && m_type == info.type();
}
