#include "spheregen.h"

#include <iostream>
#include <vector>
#include <QVector3D>
#include <random>
#include <chrono>

double sqr(double a) {
    return a * a;
}

SphereGen::SphereGen() {
    init(1, 1);
}

SphereGen::SphereGen(int w, int h) {
    init(w, h);
}

void SphereGen::_gen() const {
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed1);
    std::uniform_real_distribution<> dis_w(0, width());
    std::uniform_real_distribution<> dis_h(0, height());
    std::uniform_real_distribution<> dis_r(1, std::max(width(), height()) * 0.1);


    int s_cnt = 200;
    std::vector<QVector3D> spheres;
    for(int i = 0; i < s_cnt; i++) {
        spheres.push_back(QVector3D(dis_w(gen), dis_h(gen), dis_r(gen)));
    }

    for(int x = 0; x < height(); x++) {
        for(int y = 0; y < width(); y++) {
            double val = 0;
            for(int i = 0; i < spheres.size(); i++) {
                QVector3D q = spheres[i];
                double cur = sqr(q.z()) - (sqr(q.x() - y) + sqr(q.y() - x));
                if(cur > 0)
                    val += cur;
                //val = std::max(val, cur);
            }
            m_terrain[x][y] = val;
        }
    }

    flatter_cross(100);
    //flatter_sqrt(2);
    normalize();
}
