#include "framebuffer.h"

Framebuffer::Framebuffer(const QSize &size, bool texture) {
    m_size = size;
    m_texture_enabled = texture;

    init();
}

Framebuffer::~Framebuffer() {
    glDeleteTextures(1, &m_depth);

    if (m_texture_enabled) {
        glDeleteTextures(1, &m_texture);
    }
    glDeleteFramebuffers(1, &m_id);
}

GLuint Framebuffer::id() const {
    return m_id;
}

QSize Framebuffer::size() const {
    return m_size;
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id());
    glViewport(0, 0, m_size.width(), m_size.height());
}

GLuint Framebuffer::textureBuffer() const {
    return m_texture;
}

GLuint Framebuffer::depthBuffer() const {
    return m_depth;
}

bool Framebuffer::textureBufferEnabled() const {
    return m_texture_enabled;
}

void Framebuffer::init() {
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    initDepthTexture();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth, 0);

    if (m_texture_enabled) {
        initTexture();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture, 0);

        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);
    } else {
        glDrawBuffers(0, nullptr);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer isn't complete";
}

void Framebuffer::initDepthTexture() {
    glGenTextures(1, &m_depth);
    glBindTexture(GL_TEXTURE_2D, m_depth);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_size.width(), m_size.height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Framebuffer::initTexture() {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width(), m_size.height(), 0, GL_RGBA, GL_FLOAT, 0);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

