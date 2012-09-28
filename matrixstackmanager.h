#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <vector>

#include <QObject>
#include <QMatrix4x4>
#include <QGLShaderProgram>

/*
 * Matrix stack class.
 * Manages transformation(scale, rotation) matrices.
 * Equvivalent for deprected glPopMatrix / glPushmatrix
 */

class MatrixStackManager : public QObject {
    Q_OBJECT
public:
    typedef QMatrix4x4 Matrix;

    MatrixStackManager(QGLShaderProgram* sh_program);
    virtual ~MatrixStackManager() {};

    virtual void push();
    virtual void pop();

    const Matrix& top() const;
    Matrix& top();

    void apply(); // Send matrices product to opengl/shader
protected:
    std::vector<Matrix> m_stack; // Top is m_stack.back()
    QGLShaderProgram * m_sh_program;
};

#endif // MATRIXSTACK_H
