#include "terragen.h"

TerraGen::TerraGen() {
    init(1, 1);
}

TerraGen::TerraGen(int width, int height) {
    init(width, height);
}

TerraGen::~TerraGen() {
    //
}

const TerraGen::TTerrain &TerraGen::get() const {
    if(!m_updated) {
        m_terrain = TerraGen::TTerrain(width(), std::vector<double>(height(), 0));
        _gen();
        m_updated = true;
    }
    return m_terrain;
}

void TerraGen::gen() {
    m_terrain = TerraGen::TTerrain(width(), std::vector<double>(height(), 0));
    _gen();
    m_updated = true;

    emit terrainChanged();
}

int TerraGen::height() const {
    return m_height;
}

void TerraGen::setHeight(int h) {
    if(m_height != h)
        m_updated = false;
    m_height = h;
    emit terrainChanged();
}

int TerraGen::width() const {
    return m_width;
}

void TerraGen::setWidth(int w) {
    if(m_width != w)
        m_updated = false;
    m_width = w;
    emit terrainChanged();
}

void TerraGen::init(int w, int h) {
    m_width = w;
    m_height = h;
    m_updated = false;
}

void TerraGen::flatter_cross(int n) const {
    static const int dx[] = { 0, 0, -1, 1 };
    static const int dy[] = { 1, -1, 0, 0 };
    static const int dsz = sizeof(dx) / sizeof(int);

    TTerrain tmp;

    for (int inters = 0; inters < n; inters++) {
        tmp = m_terrain;
        for (int i = 0; i < height(); i++) {
            for (int j = 0; j < width(); j++) {
                double sum = m_terrain[i][j], cnt = 1;

                for (int p = 0; p < dsz; p++) {
                    int nx = i + dx[p], ny = j + dy[p];
                    if (nx >= 0 && nx < height() && ny >= 0 && ny < width()) {
                        cnt += 1;
                        sum += m_terrain[nx][ny];
                    }
                }
                tmp[i][j] = sum / cnt;
            }
        }
        tmp.swap(m_terrain);
    }
}

void TerraGen::flatter_sqrt(int n) const {
    double sq = 1. / n;

    for (int inters = 0; inters < n; inters++) {
        for (int i = 0; i < height(); i++) {
            for (int j = 0; j < width(); j++) {
                m_terrain[i][j] = pow(m_terrain[i][j], sq);
            }
        }
    }
}

void TerraGen::normalize() const {
    double min_val, max_val;
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            if (i == 0 && j == 0) {
                min_val = max_val = m_terrain[i][j];
            } else {
                min_val = std::min(min_val, m_terrain[i][j]);
                max_val = std::max(max_val, m_terrain[i][j]);
            }
        }
    }
    double scale = 1. / (max_val - min_val);
    for (int i = 0; i < height(); i++) {
        for (int j = 0; j < width(); j++) {
            m_terrain[i][j] = (m_terrain[i][j] - min_val) * scale;
        }
    }

}
