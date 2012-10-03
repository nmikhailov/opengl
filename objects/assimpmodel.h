#ifndef ASSIMPMODEL_H
#define ASSIMPMODEL_H

#include <QVector>
#include <QString>
#include <QGLBuffer>

#include "globject.h"

class AssimpModel : public GLObject {
    Q_OBJECT
public:
    AssimpModel(ContextManager *context);
    void loadModel(const QString& file_name);
    
protected:
    void recDraw() const;
    void _draw() const;

    QVector<QGLBuffer> m_vertex_buffer;
    QVector<QGLBuffer> m_uv_buffer;
    QVector<GLuint> m_texid;
};

#endif // ASSIMPMODEL_H
