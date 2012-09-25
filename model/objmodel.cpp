#include "glm.h"

#include "objmodel.h"

ObjModel::ObjModel(const char* filename, TextureManager* context)
        : Model(context) {
    m_model = glmReadOBJ(filename);
    // TODO: check if successfully loaded

    glmUnitize(m_model);
    glmFacetNormals(m_model);
    glmVertexNormals(m_model, 90.0);
    glmSpheremapTexture(m_model);
}

ObjModel::~ObjModel() {
    delete m_model;
}

void ObjModel::_draw() const {
    glEnable(GL_LIGHTING);
    glmDraw(m_model, GLM_SMOOTH | GLM_MATERIAL);
    glDisable(GL_LIGHTING);
//    glmDraw(m_model, GLM_SMOOTH);
}
