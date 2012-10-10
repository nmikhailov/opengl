#ifndef ASSIMPMODEL_H
#define ASSIMPMODEL_H

#include <QVector>
#include <QString>
#include <QGLBuffer>
#include <functional>

#include "globject.h"

#include <assimp/scene.h>

class AssimpModel : public GLObject {
    Q_OBJECT
public:
    AssimpModel(ContextManager *context);
    void loadModel(const QString& file_name);
    
protected:
    void _draw() const;

    QVector<QGLBuffer> m_vertex_buffer;
    QVector<QGLBuffer> m_uv_buffer;
    QVector<GLuint> m_texid;

    struct Mesh {
        void load(const aiMesh *mesh, const aiScene *scene, const QString& prefix);
        Mesh(ContextManager * cm);
        ~Mesh() {}

        QVector<GLfloat> m_vertex;
        QVector<GLfloat> m_texcoords;
        QVector<GLfloat> m_normals;

        bool m_tex_enabled = false;
        QColor m_color = Qt::white;
        GLuint m_texid;

        QGLBuffer m_buff_vert, m_buff_color, m_buff_uv, m_buff_norm;
        ContextManager * m_context;

        QString m_mesh_name;
    };

    struct Node {
        Node(ContextManager * cm);
        ~Node();

        void load(const aiNode *node, const aiScene *scene, const QString& prefix);

        QVector<Mesh*> m_meshes;
        QVector<Node*> m_nodes;

        QMatrix4x4 m_trans;
        ContextManager * m_context;
    };

    Node * m_root = nullptr;

    typedef std::function<void(Node*)> func;
    void dfs(Node * node, func fn) const;
};

#endif // ASSIMPMODEL_H
