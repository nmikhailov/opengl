#include "texturemanager.h"
#include <QtOpenGL>

TextureManager::TextureManager(QGLContext *context) {
    m_context = context;
}

TextureManager::~TextureManager() {
}

GLuint TextureManager::loadTexture(const QImage &img, int type) {
    GLuint id = m_context->bindTexture(img, type);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_FLOAT, img.data_ptr());
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    return id;
}

void TextureManager::unloadTexture(GLuint id) {
    m_context->deleteTexture(id);
}

GLuint TextureManager::getTextureByName(Texture tex) {
    QString file_name = "/home/nsl/Study/s07/graphics/qt_labs/Lab_02/models/" + tex.m_name;
    if(m_file2id.find(file_name) == m_file2id.end()) {
        m_file2id[file_name] = loadTexture(QImage(file_name), tex.type);
        qDebug() << "Requsted " << file_name;
    }
    return m_file2id[file_name];
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

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

