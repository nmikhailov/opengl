#ifndef ASSIMPMODEL_H
#define ASSIMPMODEL_H

#include <QVector>
#include <QString>
#include <QGLBuffer>
#include <functional>

#include <assimp/scene.h>

#include "globject.h"
#include "objects/group.h"
#include "scene.h"
#include "material.h"


class AssimpModel : public Group {
    Q_OBJECT
public:
    AssimpModel(const QString &file);

private:
    void load(const QString &file_name);    
};

class AssimpSubMesh : public GLObject {
    Q_OBJECT
public:
    friend class AssimpModel;

    Material material() const;
    BufferInfo vertexBuffer() const;
    GLenum primitiveType() const;

    // Get objects index, normal, tex buffers (can be not present)
    BufferInfo indexBuffer() const;
    BufferInfo normalBuffer() const;
    BufferInfo texcoordBuffer() const;

    Rect rect() const;
//
protected:
    AssimpSubMesh(aiMesh* mesh, aiScene* obj_scene);

private:
    void load(aiMesh* mesh, aiScene* obj_scene);

    Material m_material;
    QGLBuffer m_buff_index, m_buff_vertex, m_buff_normal, m_buff_tex;

    QVector<GLfloat> m_vertex;
    QVector<GLfloat> m_texcoords;
    QVector<GLfloat> m_normals;
    QVector<GLuint> m_indeces;
    QString m_mesh_name;

    Rect m_rect;
};

#endif // ASSIMPMODEL_H
