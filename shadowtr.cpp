#include "shadowtr.h"

#include <cassert>

namespace {
const GLfloat vertices[] = {
    0, 0,
    1, 0,
    0,  1,

    0,  1,
     1, 0,
     1,  1,
};
}

ShadowTr::ShadowTr() {
    init("shadow_add.vert", "shadow_add.frag");
    m_buff.create();
    m_buff.bind();
    m_buff.allocate(vertices, sizeof(vertices));
}

ShadowTr::~ShadowTr() {
}

bool ShadowTr::bind() {
    return m_program->bind();
}

void ShadowTr::release() {
    m_program->release();
}

void ShadowTr::updateLight(const std::map<LightSource *, QMatrix4x4> &, const std::map<LightSource *, GLuint> &) {
}

void ShadowTr::setMaterial(const Material &) {
}

void ShadowTr::setVertexBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("vert", type, 0, tupleSize);
    m_program->enableAttributeArray("vert");
}

void ShadowTr::setModelMatrix(const QMatrix4x4 &proj) {
    m_M = proj;
}

void ShadowTr::setViewMatrix(const QMatrix4x4 &proj) {
    m_V = proj;
}

void ShadowTr::setProjectionMatrix(const QMatrix4x4 &proj) {
    m_P = proj;
}

void ShadowTr::setIndexBuffer(QGLBuffer buff) {
    buff.bind();
}

void ShadowTr::setLight(LightSource *light, const QMatrix4x4 &model) {
    QVector3D pos = (model * QVector4D(light->position(), 0)).toVector3D();

    m_program->setUniformValue("lpos", pos);
    m_program->setUniformValue("lview", light->direction());

    double cosAngle = cos(M_PI * light->spotAngle() / 180. / 2.);
    m_program->setUniformValue("langle", (float)cosAngle);

    QMatrix4x4 pv = light->projectionMatrix() * light->viewMatrix();
    m_L = pv;
}

void ShadowTr::setShadowTexture(GLuint id) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    m_program->setUniformValue("shadowIn", 0);
}

void ShadowTr::setPreviousTexture(GLuint id) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id);

    m_program->setUniformValue("bufferIn", 1);
}

void ShadowTr::render(const GLObject *obj) {
    static const QMatrix4x4 mats(0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0);
    m_program->setUniformValue("bias", mats);
    m_program->setUniformValue("PVM", m_P * m_V * m_M);
    m_program->setUniformValue("M", m_M);
    m_program->setUniformValue("LPVM", m_L * m_M);

    m_buff.bind();
    m_program->enableAttributeArray("pos");
    m_program->setAttributeArray("pos", GL_FLOAT, 0, 2);

    GLObject::BufferInfo bf;

    bf = obj->vertexBuffer();
    if (!bf.enabled) {
        qDebug() << "Error. Vertex array is disabled for object " << ((void*)obj);
        return;
    } else {
        setVertexBuffer(bf.buff, bf.type, bf.sz);
    }

    bf = obj->indexBuffer();
    if (bf.enabled)
        setIndexBuffer(bf.buff);

    // Calc buffer size
    int sz = obj->vertexBuffer().buff.size(), d;

    switch (obj->vertexBuffer().type) {
    case GL_FLOAT:
        d = sizeof(GLfloat); break;
    case GL_DOUBLE:
        d = sizeof(GLdouble); break;
        /**/
    default:
        qDebug() << "Error, unknown vertex buffer type: " << obj->vertexBuffer().type;
        d = sizeof(GLint); break;
    }

    // Check for index buffer
    if (obj->indexBuffer().enabled) {
        //glDrawElements(obj->primitiveType(), sz / d, obj->vertexBuffer().type, nullptr);
        int ssz = obj->indexBuffer().buff.size() / sizeof(GLuint);
        glDrawElements(obj->primitiveType(), ssz, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(obj->primitiveType(), 0, sz / d);
    }
}
