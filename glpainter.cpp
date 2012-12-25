#include "glpainter.h"

GLPainter::GLPainter(Scene *scene) {
    m_scene = scene;
    init("main.vert", "main.frag");
}

GLPainter::~GLPainter() {
}

bool GLPainter::bind() {
    return m_program->bind();
}

void GLPainter::release() {
    m_program->release();
}

void GLPainter::setColor(const QColor &color) {
    m_program->setUniformValue("cl_color", color);
}

void GLPainter::setColorMode(GLPainter::EColorMode mode) {
    m_program->setUniformValue("color_mode", (int)mode);
}

void GLPainter::setVertexBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("vert", type, 0, tupleSize);
    m_program->enableAttributeArray("vert");
}

void GLPainter::setColorBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("color", type, 0, tupleSize);
    m_program->enableAttributeArray("color");
}

void GLPainter::setNormalBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("norm_buf", type, 0, tupleSize);
    m_program->enableAttributeArray("norm_buf");
}

void GLPainter::setUVBuffer(QGLBuffer buff, GLenum type, int tupleSize) {
    buff.bind();
    m_program->setAttributeBuffer("uv_buf", type, 0, tupleSize);
    m_program->enableAttributeArray("uv_buf");
}

void GLPainter::bindTexture(GLuint id) {
    m_program->setUniformValue("tex", id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

void GLPainter::setModelMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("M", proj);
    m_program->setUniformValue("M_N", proj.normalMatrix());
}

void GLPainter::setViewMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("V", proj);
}

void GLPainter::setProjectionMatrix(const QMatrix4x4 &proj) {
    m_program->setUniformValue("P", proj);
}

void GLPainter::setIndexBuffer(QGLBuffer buff) {
    buff.bind();
}

void GLPainter::updateLight(const std::map<LightSource*, QMatrix4x4>  &lights, const std::map<LightSource*, GLuint>  &lights_tex) {
    int id = 0;
    for (auto rec: lights) {
        LightSource* light = rec.first;
        QString name = QString("lights[%1].").arg(id);
        QString name_shadow = QString("shadows[%1]").arg(id);

        QVector3D pos = (rec.second * QVector4D(light->position(), 0)).toVector3D();

        m_program->setUniformValue((name + "diffuse").toLatin1().data(),
                                   light->diffuseColor());
        m_program->setUniformValue((name + "position").toLatin1().data(),
                                   pos);
        m_program->setUniformValue((name + "att").toLatin1().data(),
                                    light->attenuation());
        m_program->setUniformValue((name + "direction").toLatin1().data(),
                                    light->direction());

        double cosAngle = cos(M_PI * light->spotAngle() / 180. / 2.);
        m_program->setUniformValue((name + "cosAngle").toLatin1().data(), (float)cosAngle);

        QMatrix4x4 pv = light->projectionMatrix(1) * light->viewMatrix();
        m_program->setUniformValue((name + "PV_light").toLatin1().data(), pv);
        //
        glActiveTexture(GL_TEXTURE1 + id);
        glBindTexture(GL_TEXTURE_2D, lights_tex.at(light));
        m_program->setUniformValue(name_shadow.toAscii().data(), id + 1);
        //

        id++;
    }

    m_program->setUniformValue("lightCnt", id);

}

void GLPainter::render(const GLObject *obj) {
    static const QMatrix4x4 mats(0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0);
    m_program->setUniformValue("bias", mats);

    GLObject::BufferInfo bf;

    bf = obj->vertexBuffer();
    if (!bf.enabled) {
        qDebug() << "Error. Vertex array is disabled for object " << ((void*)obj);
        return;
    } else {
        setVertexBuffer(bf.buff, bf.type, bf.sz);
    }

    bf = obj->indexBuffer();
    setIndexBuffer(bf.buff);

    bf = obj->texcoordBuffer();
    setUVBuffer(bf.buff, bf.type, bf.sz);

    bf = obj->normalBuffer();
    setNormalBuffer(bf.buff, bf.type, bf.sz);

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

    setMaterial(obj->material());

    // Check for index buffer
    if (obj->indexBuffer().enabled) {
        //glDrawElements(obj->primitiveType(), sz / d, obj->vertexBuffer().type, nullptr);
        int ssz = obj->indexBuffer().buff.size() / sizeof(GLuint);
        glDrawElements(GL_TRIANGLES, ssz, GL_UNSIGNED_INT, nullptr);
    } else {
        //glDrawArrays(obj->primitiveType(), 0, sz / d);
    }
}

void GLPainter::setMaterial(const Material &m) {
    if (m.type() == Material::C_TEXTURE) {
        setColorMode(CM_TEXTURE);

        glActiveTexture(GL_TEXTURE0);
        GLuint id = m_scene->textureManager()->getTexture(m.texture());
        glBindTexture(GL_TEXTURE_2D, id);
        m_program->setUniformValue("tex", 0);

    } else if(m.type() == Material::C_UNIFROM) {
        setColorMode(CM_ONE_COLOR);
        setColor(m.color());
    }
}
