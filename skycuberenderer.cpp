#include "skycuberenderer.h"

#include "objects/globject.h"


void SkycubeRenderer::updateLight(const std::map<LightSource *, QMatrix4x4> &lights, const std::map<LightSource *, GLuint> &lights_tex) {
    // Nothing to do
}

void SkycubeRenderer::render(const GLObject *obj) {
    setMaterial(obj->material());

    m_program->setUniformValue("PVM", m_proj * m_view * m_model);
    //
    obj->vertexBuffer().buff.bind();
    m_program->setAttributeArray("vertex", GL_FLOAT, 0, 3);
    m_program->enableAttributeArray("vertex");

    if (obj->indexBuffer().enabled) {
        obj->indexBuffer().buff.bind();
    }

    int ssz = obj->indexBuffer().buff.size() / sizeof(GLuint);
    glDrawElements(GL_QUADS, ssz, GL_UNSIGNED_INT, nullptr);
}

void SkycubeRenderer::setMaterial(const Material &m) {
    //m_tex_id = m_scene->textureManager()->getTextureByName(m.texture());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex_id);
    m_program->setUniformValue("cubemap", 0);
}

bool SkycubeRenderer::bind() {
    return m_program->bind();
}

void SkycubeRenderer::release() {
    m_program->release();
}

void SkycubeRenderer::init2() {
    glGenTextures (1, &m_tex_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    QImage image = QImage("/home/nsl/Study/s07/graphics/qt_labs/Lab_02/models/skybox.png");
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
    //int val[] = {6, 6,    1, 9,   6, 6};

    for (int i = 0; i < 6; i++) {
        glTexImage2D(key[i], 0, GL_RGBA, images[val[i]].width(), images[val[i]].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, images[val[i]].bits());
    }

}

void SkycubeRenderer::setProjectionMatrix(const QMatrix4x4 &proj) {
    m_proj = proj;
}

void SkycubeRenderer::setViewMatrix(const QMatrix4x4 &proj) {
    m_view = proj;
}

void SkycubeRenderer::setModelMatrix(const QMatrix4x4 &proj) {
    m_model = proj;
}

SkycubeRenderer::SkycubeRenderer(Scene *s) {
    m_scene = s;
    init("skybox.vert", "skybox.frag");
    init2();
}
