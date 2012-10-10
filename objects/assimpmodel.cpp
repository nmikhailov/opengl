#include "assimpmodel.h"

#include <QDebug>

#include <boost/iterator/zip_iterator.hpp>

#include <assimp/Importer.hpp>
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
//QString prefix = "/home/nsl/mc.blend";
void AssimpModel::loadModel(const QString &file_name) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile((prefix + file_name).toLatin1().data(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals |
                                             //aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);
    if (!scene) {
        qDebug() << "Error: " << importer.GetErrorString();
        return;
    }

    // load
    m_root = new Node(m_context);
    m_root->load(scene->mRootNode, scene, prefix);

    QVector3D min = QVector3D(1, 1, 1) * 1e9, max = -min, t;
    auto fn_draw = [&](Node* node) {
        for (Mesh * mesh : node->m_meshes) {
            for (size_t i = 0; i < mesh->m_vertex.size(); i += 3) {
                min.setX(std::min(mesh->m_vertex[i + 0], (float) min.x()));
                min.setY(std::min(mesh->m_vertex[i + 1], (float) min.y()));
                min.setZ(std::min(mesh->m_vertex[i + 2], (float) min.z()));

                max.setX(std::max(mesh->m_vertex[i + 0], (float) max.x()));
                max.setY(std::max(mesh->m_vertex[i + 1], (float) max.y()));
                max.setZ(std::max(mesh->m_vertex[i + 2], (float) max.z()));
            }
        }
    };
    dfs(m_root, fn_draw);

    // Scales object to fit it in 1x1x1 box and moves it to 0,0,0
    t = max - min;
    double r = std::max({t.x(), t.y(), t.z()});
    m_root->m_trans.scale(1. / r);
    m_root->m_trans.translate(-(max + min) / 2);
}

void AssimpModel::_draw() const {
    ColorShader sh = m_context->shaderManager()->setActiveShader<ColorShader>();
    sh.setColorMode(ColorShader::CM_ONE_COLOR);
    sh.setColor(Qt::white);

    sh.bindTexture(m_context->textureManager()->getTextureByName(prefix + "airplane2/Diffuse.tga"));

    auto fn_draw = [&](Node* node) {
        for (Mesh * mesh : node->m_meshes) {
            if (mesh->m_tex_enabled) {
                sh.setColorMode(ColorShader::CM_TEXTURE);
                sh.setUVBuffer(mesh->m_buff_uv);
            } else {
                sh.setColorMode(ColorShader::CM_ONE_COLOR);
                sh.setColor(mesh->m_color);
            }
            sh.setVertexBuffer(mesh->m_buff_vert);
            //
            glDrawArrays(GL_QUADS, 0, mesh->m_buff_vert.size() / sizeof(GLfloat));
        }
    };

    dfs(m_root, fn_draw);
}

void AssimpModel::dfs(AssimpModel::Node *node, AssimpModel::func fn) const {
    m_context->matrixStackManager()->push();
    m_context->matrixStackManager()->top() *= node->m_trans;
    m_context->matrixStackManager()->apply();

    fn(node);

    for (auto * n : node->m_nodes) {
        dfs(n, fn);
    }
    m_context->matrixStackManager()->pop();
    m_context->matrixStackManager()->apply();
}

void AssimpModel::Node::load(const aiNode *node, const aiScene *scene, const QString &prefix) {
    // Current transformation matrix for node and all subnodes
    m_trans = conv(&node->mTransformation);

    // Process all meshes in node
    for(size_t mesh_id = 0; mesh_id < node->mNumMeshes; mesh_id++) {
        Mesh * m = new AssimpModel::Mesh(m_context);
        m->load(scene->mMeshes[node->mMeshes[mesh_id]], scene, prefix);
        m_meshes << m;
    }
    // Process all subnodes
    for(size_t node_id = 0; node_id < node->mNumChildren; node_id++) {
        Node * n = new AssimpModel::Node(m_context);
        n->load(node->mChildren[node_id], scene, prefix);
        m_nodes << n;
    }
}

AssimpModel::Node::Node(ContextManager *cm) {
    m_context = cm;
}

AssimpModel::Node::~Node() {
    for(auto *node: m_nodes)
        delete node;
    for(auto *mesh: m_meshes)
        delete mesh;
}


void AssimpModel::Mesh::load(const aiMesh *mesh, const aiScene *scene, const QString &prefix) {
    // Try to load texture(s) for mesh
    aiMaterial * mat = scene->mMaterials[mesh->mMaterialIndex];
    unsigned int tex_cnt = mat->GetTextureCount(aiTextureType_DIFFUSE);
    m_texid = 0;
    if(tex_cnt > 0) {
        aiString * str = new aiString();
        mat->GetTexture(aiTextureType_DIFFUSE, 0, str);

        qDebug() << "TEX:" << str->C_Str();
        m_texid = m_context->textureManager()->getTextureByName(prefix + str->C_Str());
    }

    // Vertexes, tex coords and normals
    m_normals.clear();
    m_vertex.clear();
    m_texcoords.clear();
    for (size_t v_id = 0; v_id < mesh->mNumVertices; v_id++) {
        aiVector3D v = mesh->mVertices[v_id], vn = mesh->mNormals[v_id];
        m_vertex << v.x << v.y << v.z;
        m_normals << vn.x << vn.y << vn.z;

        // TODO: Process all textures(currently only first texture is proceeded)
        if (mesh->HasTextureCoords(0)) {
            aiVector3D t = mesh->mTextureCoords[0][v_id];
            m_texcoords << t.x << t.y;
        } else {
            m_texcoords << 0 << 0;
        }
    }

    m_tex_enabled = mesh->HasTextureCoords(0);

    aiString name;
    aiColor4D color;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mat->Get(AI_MATKEY_NAME, name);
    m_color = QColor(color.r * 255., color.g * 255., color.b * 255.);
    //qDebug() << name.C_Str() << m_color;

    // Make buffers
    m_buff_vert.create();
    m_buff_color.create();
    m_buff_uv.create();
    m_buff_norm.create();

    m_buff_vert.bind();
    m_buff_vert.allocate(m_vertex.constData(), m_vertex.size() * sizeof(GLfloat));

    m_buff_uv.bind();
    m_buff_uv.allocate(m_texcoords.constData(), m_texcoords.size() * sizeof(GLfloat));

    m_buff_norm.bind();
    m_buff_norm.allocate(m_normals.constData(), m_normals.size() * sizeof(GLfloat));
}

AssimpModel::Mesh::Mesh(ContextManager *cm) {
    m_context = cm;
}
