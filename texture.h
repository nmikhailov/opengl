#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QString>
#include <QtOpenGL>

class TextureInfo {
public:
    enum T_TYPE {T_2D, T_CUBIC};

    TextureInfo();
    explicit TextureInfo(const QString& file_name, T_TYPE type = T_2D);

    QString fileName() const;
    void setFileName(const QString& file_name);

    T_TYPE type() const;
    void setType(T_TYPE type);

    bool operator==(const TextureInfo &info) const;
protected:
    QString m_name;
    T_TYPE m_type = T_2D;
};


namespace std {


template<>
struct hash<TextureInfo> {
    size_t operator()(const TextureInfo& val) const noexcept {
        size_t hash = std::hash<std::string>()(val.fileName().toStdString());
        hash ^= val.type();
        return hash;
    }
};

}


#endif // TEXTURE_H
