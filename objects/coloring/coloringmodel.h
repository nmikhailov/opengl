#ifndef COLORINGMODEL_H
#define COLORINGMODEL_H

#include <QColor>

class ColoringModel {
public:
    virtual QColor operator()(double val) const = 0;
    virtual ~ColoringModel() {}
    virtual QString name() const = 0;
};

class HueColoringModel : public ColoringModel {
public:
    virtual QColor operator()(double val) const {
        return QColor::fromHsvF(1 - val, 0.8, 0.8 * val);
    }

    virtual QString name() const {
        return "Hue CM";
    }
};

class HeightColorModel : public ColoringModel {
    virtual QColor operator()(double val) const {
        if(val > 0.9) {
            return QColor::fromHsvF(0, 0, 0.9 * val);
        } else if(val > 0.3) {
            return QColor::fromHsvF(0.3, 0.8, 0.8 * val);
        }
        return QColor::fromRgbF(0, 0, 0.6);
    }

    virtual QString name() const {
        return "Height CM";
    }
};

#endif // COLORINGMODEL_H
