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
    m_tex_id = m_scene->textureManager()->getTexture(m.texture());

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
}
