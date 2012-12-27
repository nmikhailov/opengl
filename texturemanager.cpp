#include "texturemanager.h"
#include <QtOpenGL>
#include <cassert>

TextureManager::TextureManager(QGLContext *context) {
    m_context = context;
}

TextureManager::~TextureManager() {
}

GLuint TextureManager::loadTexture(const QImage &img, TextureInfo::T_TYPE type) {
    QImage image = img;

    GLuint id;// = m_context->bindTexture(img, type);
    glGenTextures(1, &id);

    GLuint internal;
    switch (type) {
    case TextureInfo::T_2D:
        internal = GL_TEXTURE_2D;
        break;
    case TextureInfo::T_CUBIC:
        internal = GL_TEXTURE_CUBE_MAP;
        break;
    default:
        qDebug() << "Unknown texture type " << type << " ignoring.";
        return 0;
    }

    glBindTexture(internal, id);

    if (type == TextureInfo::T_2D) {
        glTexImage2D(internal, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    } else if (type == TextureInfo::T_CUBIC) {
        QImage images[12];
        int width = image.width() / 4;
        int height = image.width() / 4;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                QRect rect = QRect(width * j, height * i, width, height);

                images[i * 4 + j] = QGLWidget::convertToGLFormat(image.copy(rect)).mirrored(false, true);
            }
        }
        int key[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};
        int val[] = {6, 4, 1, 9, 5, 7};

        for (int i = 0; i < 6; i++) {
            glTexImage2D(key[i], 0, GL_RGBA, images[val[i]].width(), images[val[i]].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, images[val[i]].bits());
        }
    }

    glTexParameteri(internal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(internal, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(internal, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(internal, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return id;
}

void TextureManager::unloadTexture(GLuint id) {
    m_context->deleteTexture(id);
}

GLuint TextureManager::getTexture(TextureInfo tex) {
    QString file_name = "/home/nsl/Study/s07/graphics/qt_labs/Lab_02/models/" + tex.fileName();

    if (m_cache.find(tex) == m_cache.end()) {
        m_cache[tex] = loadTexture(QImage(file_name), tex.type());
        qDebug() << "Requsted " << file_name;
    }

    return m_cache[tex];
}
