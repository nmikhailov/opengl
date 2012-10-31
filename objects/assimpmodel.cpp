#include "assimpmodel.h"

#include <QDebug>
#include <queue>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

QMatrix4x4 conv(const aiMatrix4x4 * m) {
    return QMatrix4x4(m->a1, m->b1, m->c1, m->d1,
                      m->a2, m->b2, m->c2, m->d2,
                      m->a3, m->b3, m->c3, m->d3,
                      m->a4, m->b4, m->c4, m->d4);
}

QString prefix = "/home/nsl/Study/s07/graphics/qt_labs/Lab_02/models/";

AssimpModel::AssimpModel(Scene *scene, const QString &file) : Group(scene) {
    load(file);
}


void AssimpModel::load(const QString &file_name) {
    Assimp::Importer importer;
    // TODO: fix const cast
    aiScene* scene = (aiScene*)importer.ReadFile((prefix + file_name).toLatin1().data(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_GenSmoothNormals  |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType |
                                             aiProcess_OptimizeMeshes |
                                             aiProcess_OptimizeGraph |
                                             0);
    if (!scene) {
        qDebug() << "Error: " << importer.GetErrorString();
        return;
    }

    // Process all object nodes
    std::queue<std::pair<aiNode*, Group*> > q;
    q.push(std::make_pair(scene->mRootNode, this));

    while (!q.empty()) {
        aiNode* cur_node = q.front().first;
        Group* cur_group = q.front().second;
        q.pop();
        //

        // Current transformation matrix for node and all subnodes
        m_base_transform = conv(&cur_node->mTransformation);

        // Process all meshes in node
        for (size_t mesh_id = 0; mesh_id < cur_node->mNumMeshes; mesh_id++) {
            aiMesh* nmesh = scene->mMeshes[cur_node->mMeshes[mesh_id]];

            AssimpSubMesh *smesh = m_scene->newObject<AssimpSubMesh>(nmesh, scene);
            cur_group->add(smesh);
        }
        // Process all subnodes
        for (size_t node_id = 0; node_id < cur_node->mNumChildren; node_id++) {
            Group* ngroup = m_scene->newGroup<Group>();
            cur_group->add(ngroup);

            q.push(std::make_pair(cur_node->mChildren[node_id], ngroup));
        }
    }
}


AssimpSubMesh::AssimpSubMesh(Scene *scene, aiMesh *mesh, aiScene *obj_scene) : GLObject(scene) {
    load(mesh, obj_scene);
}

void AssimpSubMesh::load(aiMesh *mesh, aiScene *obj_scene) {
    m_mesh_name = mesh->mName.C_Str();

    // Load texture(s) for mesh
    aiMaterial * mat = obj_scene->mMaterials[mesh->mMaterialIndex];
    unsigned int tex_cnt = mat->GetTextureCount(aiTextureType_DIFFUSE);

    if(tex_cnt > 0) {
        aiString * str = new aiString();
        mat->GetTexture(aiTextureType_DIFFUSE, 0, str);

        qDebug() << "TEX:" << str->C_Str();
        //m_texid = m_context->textureManager()->getTextureByName(prefix + str->C_Str());

        //m_material.setTexture();
        m_material.setType(Material::C_TEXTURE);
    } else {
        m_material.setType(Material::C_UNIFROM);
    }

    // Vertexes, tex coords and normals
    m_normals.clear();
    m_vertex.clear();
    m_texcoords.clear();

    for (size_t v_id = 0; v_id < mesh->mNumVertices; v_id++) {
        aiVector3D v = mesh->mVertices[v_id], vn = mesh->mNormals[v_id];
        m_vertex << v.x << v.y << v.z;
        m_normals << vn.x << vn.y << vn.z;

        m_rect.xMax = std::max(v.x, m_rect.xMax);
        m_rect.xMin = std::max(v.x, m_rect.xMin);

        m_rect.yMax = std::max(v.y, m_rect.yMax);
        m_rect.yMin = std::max(v.y, m_rect.yMin);

        m_rect.zMax = std::max(v.z, m_rect.zMax);
        m_rect.zMin = std::max(v.z, m_rect.zMin);

        // TODO: Process all textures(currently only first texture is proceeded)
        if (mesh->HasTextureCoords(0)) {
            aiVector3D t = mesh->mTextureCoords[0][v_id];
            m_texcoords << t.x << t.y;
        } else {
            m_texcoords << 0 << 0;
        }
    }

    //m_tex_enabled = mesh->HasTextureCoords(0);

    aiString name;
    aiColor4D color;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    mat->Get(AI_MATKEY_NAME, name);
    QColor qcolor = QColor(color.r * 255., color.g * 255., color.b * 255.);
    m_material.setType(Material::C_UNIFROM);
    m_material.setColor(qcolor);

    //qDebug() << name.C_Str() << m_color;

    // Load index buffers
    m_indeces.clear();
    for (size_t f_id = 0; f_id < mesh->mNumFaces; f_id++) {
        aiFace* face = &mesh->mFaces[f_id];

        for (size_t i = 0; i < face->mNumIndices; i++) {
            m_indeces << face->mIndices[i];
        }
    }

    // Make buffers
    m_buff_vertex.create();
    m_buff_vertex.bind();
    m_buff_vertex.allocate(m_vertex.constData(), m_vertex.size() * sizeof(GLfloat));

    m_buff_tex.create();
    m_buff_tex.bind();
    m_buff_tex.allocate(m_texcoords.constData(), m_texcoords.size() * sizeof(GLfloat));

    m_buff_normal.create();
    m_buff_normal.bind();
    m_buff_normal.allocate(m_normals.constData(), m_normals.size() * sizeof(GLfloat));

    // Make index buffer
    m_buff_index = QGLBuffer(QGLBuffer::IndexBuffer);
    m_buff_index.create();
    m_buff_index.bind();
    m_buff_index.allocate(m_indeces.constData(), m_indeces.size() * sizeof(GLuint));
}

GLenum AssimpSubMesh::primitiveType() const {
    return GL_TRIANGLES;
}

Material AssimpSubMesh::material() const {
    return m_material;
}

GLObject::BufferInfo AssimpSubMesh::vertexBuffer() const {
    return BufferInfo(true, GL_FLOAT, 3, m_buff_vertex);
}

GLObject::BufferInfo AssimpSubMesh::indexBuffer() const {
    //return BufferInfo(false);
    return BufferInfo(true, GL_UNSIGNED_INT, 1, m_buff_index);
}

GLObject::BufferInfo AssimpSubMesh::normalBuffer() const {
    return BufferInfo(true, GL_FLOAT, 3, m_buff_normal);
}

GLObject::BufferInfo AssimpSubMesh::texcoordBuffer() const {
    return BufferInfo(true, GL_FLOAT, 2, m_buff_tex);
}

Rect AssimpSubMesh::rect() const {
    return m_rect;
}
