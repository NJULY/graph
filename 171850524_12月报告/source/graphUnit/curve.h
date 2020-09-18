#ifndef CURVE_H
#define CURVE_H

#include "graphUnit.h"

class Curve : public graphUnit
{
public:
    Curve();
    Curve(QColor color);
    void paint_Bezier(QPixmap *canvas);
    void paint_B_Spline(QPixmap *canvas);
    double Bik(int i, int k, double u);

    QVector<QPoint> pointVector;
    double* U;
    int k;
    int curveAlgorithm=0;
};

#endif // CURVE_H
