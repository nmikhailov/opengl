#include "assimpmodel.h"

#include <QDebug>

#include <boost/iterator/zip_iterator.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

QMatrix4x4 conv(const aiMatrix4x4 * m) {
    return QMatrix4x4(m->a1, m->b1, m->c1, m->d1,
                      m->a2, m->b2, m->c2, m->d2,
                      m->a3, m->b3, m->c3, m->d3,
                      m->a4, m->b4, m->c4, m->d4);
}

AssimpModel::AssimpModel(ContextManager *context) : GLObject(context) {
}

QString prefix = "/home/nsl/Study/s07/graphics/qt_labs/Lab_02/models/";
void AssimpModel::loadModel(const QString &file_name) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile((prefix + file_name).toLatin1().data(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             //aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);
    if (!scene) {
        qDebug() << "Error: " << importer.GetErrorString();
        return;
    }
    qDebug() << scene->mRootNode->mChildren[0];
    for(int i = 0; i < scene->mRootNode->mNumChildren; i++) {
    aiNode * root = scene->mRootNode->mChildren[i];
    QMatrix4x4 m = conv(&root->mTransformation);

    //setRotation(m);
    //m.setToIdentity();
    //m.rotate(-90, 1, 0, 0);
    m.scale(QVector3D(1, 1, 1) * 1e-3);
    //qDebug() << m;
    for(size_t mesh_id = 0; mesh_id < root->mNumMeshes; mesh_id++) {
        aiMesh * mesh = scene->mMeshes[root->mMeshes[mesh_id]];
        int cont = aiGetMaterialTextureCount(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
        if(cont > 0) {
            aiString * str = new aiString();
            aiGetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, 0, str, 0, 0, 0, 0, 0, 0);
            qDebug() << "TEX:" << str->C_Str();
            int id = m_context->textureManager()->getTextureByName(prefix + "airplane2/" + str->C_Str());
            m_texid.append(id);
        } else {
            m_texid.append(0);
        }
        // Vertexes
        QVector<GLfloat> vertexes, uv;
        double min_x, max_x, min_z, max_z;
        for(size_t v_id = 0; v_id < mesh->mNumVertices; v_id++) {
            aiVector3D v = mesh->mVertices[v_id];
            QVector4D vec(v.x, v.y, v.z, 0);
            vec = m * vec;
            vertexes << vec.x() << vec.y() << vec.z();
            if(i == 5) {
               // qDebug() << vec;
                if(v_id == 0) {
                    min_x = max_x = vec.x();
                    min_z = max_z = vec.z();
                }
                min_z = std::min(min_z, vec.z());
                max_z = std::max(max_z, vec.z());

                min_x = std::min(min_x, vec.x());
                max_x = std::max(max_x, vec.x());
            }
            if(mesh->HasTextureCoords(0)) {
                aiVector3D t = mesh->mTextureCoords[0][v_id];
                uv << t.x << t.y;
            } else {
                uv << 0 << 0;
            }
        }
       // qDebug() << "-> " << min_x << max_x << min_z << max_z;
        QGLBuffer b;
        b.create();
        b.bind();
        b.allocate(vertexes.constData(), vertexes.size() * sizeof(GLfloat));
        m_vertex_buffer.push_back(b);
        // uv
        QGLBuffer b2;
        b2.create();
        b2.bind();
        b2.allocate(uv.constData(), uv.size() * sizeof(GLfloat));
        m_uv_buffer.push_back(b2);
    }
    }
}

void AssimpModel::_draw() const {
    ColorShader sh = m_context->shaderManager()->setActiveShader<ColorShader>();
    sh.setColorMode(ColorShader::CM_TEXTURE);
    sh.setColor(Qt::blue);

    static double z = 0;
    sh.bindTexture(m_context->textureManager()->getTextureByName(
                       prefix + "/airplane2/Diffuse.tga"));
    for(size_t i = 0; i < m_vertex_buffer.size(); i++) {
        QGLBuffer a = m_vertex_buffer[i], b = m_uv_buffer[i];
        sh.setVertexBuffer(a);
        //sh.bindTexture(m_texid[i]);
        sh.setUVBuffer(b);
        //sh.setColorMode(ColorShader::CM_ONE_COLOR);

        //
        if(i != 15){
            glDrawArrays(GL_TRIANGLES, 0, a.size() / sizeof(GLfloat));
        }else{
            QVector3D diff(-0.01, 0, 0.05);
            m_context->matrixStackManager()->push();
            m_context->matrixStackManager()->top().translate(-diff);
            m_context->matrixStackManager()->top().rotate(z, 0, 1, 0);
            m_context->matrixStackManager()->top().translate(diff);


            m_context->matrixStackManager()->apply();
            glDrawArrays(GL_TRIANGLES, 0, a.size() / sizeof(GLfloat));

            m_context->matrixStackManager()->pop();
            m_context->matrixStackManager()->apply();
            z += 100;
        }
    }

}

