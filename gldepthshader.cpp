#include "gldepthshader.h"

GLDepthShader::GLDepthShader(Scene *scene) : m_program(new QGLShaderProgram()) {
    m_scene = scene;
    init("shadow_map.vert", "shadow_map.frag");
}

GLDepthShader::~GLDepthShader() {
}

void GLDepthShader::init(QString vert, QString frag) {
    QString prefix = ":/s/shaders/";

    m_program->addShaderFromSourceFile(QGLShader::Vertex, prefix + vert);
    m_program->addShaderFromSourceFile(QGLShader::Fragment, prefix + frag);
    m_program->link();
}

bool GLDepthShader::bind() {
    return m_program->bind();
}

void GLDepthShader::release() {
    m_program->release();
}

void GLDepthShader::setVertexBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("vert", type, 0, tupleSize);
    m_program->enableAttributeArray("vert");
}

void GLDepthShader::setModelMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("M", proj);
    m_program->setUniformValue("M_N", proj.normalMatrix());
}

void GLDepthShader::setViewMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("V", proj);
}

void GLDepthShader::setProjectionMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("P", proj);
}

void GLDepthShader::setIndexBuffer(QGLBuffer buff) {
    buff.bind();
}

void GLDepthShader::render(const GLObject *obj) {
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
        glDrawElements(GL_TRIANGLES, ssz, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(obj->primitiveType(), 0, sz / d);
    }
}
