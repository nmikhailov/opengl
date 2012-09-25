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

void ObjModel::draw() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    glPushMatrix();
    glRotated(m_rotation.x(), 1, 0, 0);
    glRotated(m_rotation.y(), 0, 1, 0);
    glRotated(m_rotation.z(), 0, 0, 1);

    glScaled(m_scale.x(), m_scale.y(), m_scale.z());

    glTranslated(m_position.x(), m_position.y(), m_position.z());

    _draw();
    glPopMatrix();
}

void ObjModel::_draw() const {
    glEnable(GL_LIGHTING);
    glmDraw(m_model, GLM_SMOOTH | GLM_MATERIAL);
    glDisable(GL_LIGHTING);
//    glmDraw(m_model, GLM_SMOOTH);
}
