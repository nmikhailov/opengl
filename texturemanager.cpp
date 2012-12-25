#include "texturemanager.h"
#include <QtOpenGL>

TextureManager::TextureManager(QGLContext *context) {
    m_context = context;
}

TextureManager::~TextureManager() {
}

GLuint TextureManager::loadTexture(const QImage &img, TextureInfo::T_TYPE type) {
    QImage image = QGLWidget::convertToGLFormat(img);

    GLuint id;// = m_context->bindTexture(img, type);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

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
        glTexImage2D(internal, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
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

    glTexParameteri(internal, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

GLuint TextureManager::genFBTexture(int width, int heigth) {
    GLuint res;
    glGenTextures(1, &res);
    glBindTexture(GL_TEXTURE_2D, res);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_FLOAT, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return res;
}

GLuint TextureManager::genFBDepthTexture(int width, int heigth) {
    GLuint res;
    glGenTextures(1, &res);
    glBindTexture(GL_TEXTURE_2D, res);


//    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, heigth, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, heigth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, res, 0);

    return res;
}

GLuint TextureManager::genDepthFramebuffer(const QSize &size, GLuint &tex_id) {
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    //renderedTexture = m_texture_manager->genFBTexture(1024, 1024);
    tex_id = genFBDepthTexture(size.width(), size.height());


    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_id, 0);

    // Set the list of draw buffers.
    //GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, DrawBuffers);
    glDrawBuffers(0, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer isn't complete";

    return FramebufferName;
}
