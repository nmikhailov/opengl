#include "randomterragen.h"
#include <random>


RandomTerraGen::RandomTerraGen(int width, int height) {
    init(width, height);
}

RandomTerraGen::~RandomTerraGen() {
}

QString RandomTerraGen::name() const {
    return "RandomTerraGen";
}

void RandomTerraGen::_gen() const {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis(0, 1);

    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            double px = abs(i - height() / 2.) / (height() / 2.);
            double py = abs(j - width() / 2.) / (width() / 2.);
            double val = dis(gen);

            if (rand() % 100 >= std::max(px, py) * 50) {
                m_terrain[i][j] = val;
            }
        }
    }

    flatter_cross(80);
    normalize();
}
