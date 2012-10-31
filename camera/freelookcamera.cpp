#include <QKeyEvent>
#include <QMouseEvent>
#include "freelookcamera.h"

FreeLookCamera::FreeLookCamera(Scene *scene) : Camera(scene) {
    m_normal = QVector3D(0, 1, 0);
    //
    m_keys.resize(6);
    m_keys[0].keys << Qt::Key_W << Qt::Key_Up;
    m_keys[0].diff = QVector3D(1, 0, 0);

    m_keys[1].keys << Qt::Key_S << Qt::Key_Down;
    m_keys[1].diff = QVector3D(-1, 0, 0);

    m_keys[2].keys << Qt::Key_A << Qt::Key_Left;
    m_keys[2].diff = QVector3D(0, 1, 0);

    m_keys[3].keys << Qt::Key_D << Qt::Key_Right;
    m_keys[3].diff = QVector3D(0, -1, 0);

    m_keys[4].keys << Qt::Key_Shift;
    m_keys[4].diff = QVector3D(0, 0, -1);

    m_keys[5].keys << Qt::Key_Space;
    m_keys[5].diff = QVector3D(0, 0, 1);
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    //m_timer.start(50);
}

FreeLookCamera::~FreeLookCamera() {
}

QVector3D FreeLookCamera::viewVector() const {
    return m_view;
}

void FreeLookCamera::setViewVector(const QVector3D &view) {
    m_view = view.normalized();
}

QVector3D FreeLookCamera::normal() const {
    return m_normal;
}

void FreeLookCamera::setNormal(const QVector3D &normal) {
    m_normal = normal.normalized();
}

QMatrix4x4 FreeLookCamera::projectionMatrix() const {
    QMatrix4x4 mat;
    mat.perspective(90, m_screen_size.x() / m_screen_size.y(), 1e-1, 1e6);
    return mat;
}

QMatrix4x4 FreeLookCamera::viewMatrix() const {
    QMatrix4x4 m;
    m.lookAt(m_pos, m_pos + m_view, m_normal);
    return m;
}

void FreeLookCamera::mouseMoveEvent(QMouseEvent *event, QVector2D new_pos) {
    static bool first_event = true;
    if (!first_event) {
        QVector2D diff = (QVector2D(event->pos()) - m_last_mouse_pos) / 5.;
        QMatrix4x4 rot;
        QVector3D norm = QVector3D::normal(m_normal, m_view), t;

        rot.rotate(diff.y(), norm);
        rot.rotate(diff.x(), -m_normal);

        t = rot * m_view;

        m_view = rot * m_view;
    }
    m_last_mouse_pos = new_pos;
    first_event = false;
}

void FreeLookCamera::keyPressEvent(QKeyEvent *event) {
    forEachFilter([&](KeyFilter &kf) {kf.enabled = true;}, event->key());
}

void FreeLookCamera::keyReleaseEvent(QKeyEvent *event) {
    forEachFilter([&](KeyFilter &kf) {kf.enabled = false;}, event->key());
}

void FreeLookCamera::tick() {
    for (auto &kf : m_keys) {
        if (kf.enabled) {
            if (kf.diff.x() != 0)
                m_pos += kf.diff.x() * m_view * m_speed;

            if (kf.diff.y() != 0)
                m_pos += kf.diff.y() * QVector3D::normal(m_normal, m_view) * m_speed;

            if (kf.diff.z() != 0)
                m_pos.setY(m_pos.y() + kf.diff.z() * m_speed);
        }
    };
}

void FreeLookCamera::forEachFilter(FreeLookCamera::fn f, int ekey) {
    for (auto &kf : m_keys) {
        for (auto &key : kf.keys) {
            if (key == ekey) {
                f(kf);
                return;
            }
        }
    }
}
