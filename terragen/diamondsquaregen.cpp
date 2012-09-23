#include "diamondsquaregen.h"
#include <iostream>

DiamondSquareGen::~DiamondSquareGen() {
}


void DiamondSquareGen::_gen() const {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis(0, 0.5);

    m_terrain[0][0] = 0;
    m_terrain[0][width() - 1] = 0;
    m_terrain[height() - 1][0] = 0;
    m_terrain[height() - 1][width() - 1] = 0;
    fn(0, width() - 1, 0, height() - 1, 10);

    //flatter_sqrt(2);
    flatter_cross(20);

    normalize();
}

void DiamondSquareGen::fn(int l, int r, int b, int t, int iter) const {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis(0, 1);

    //std::cout << "in " << l << " " << r << " " << t << " " << b << std::endl;
    //std::cout.flush();
    if(abs(r - l) < 2 || abs(t - b) < 2)
        return;

    double A = m_terrain[t][l], B = m_terrain[t][r], C = m_terrain[b][l], D = m_terrain[b][r];


    int midX = (t + b) / 2;
    int midY = (l + r) / 2;

    m_terrain[midX][l] = (A + C) / 2.;
    m_terrain[midX][r] = (B + D) / 2.;

    m_terrain[t][midY] = (A + B) / 2.;
    m_terrain[b][midY] = (C + D) / 2.;


    double ad = rand() % (abs(r - l));


    m_terrain[midX][midY] = (A + B + C + D) / 4. + ad;

    fn(l, midY, midX, t, iter * 2);
    fn(midY, r, b, midX, iter * 2);

    fn(l, midY, b, midX, iter * 2);
    fn(midY, r, midX, t, iter * 2);
}
