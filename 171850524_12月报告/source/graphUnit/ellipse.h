#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "graphUnit.h"

class Ellipse : public graphUnit
{
public:
    Ellipse();
    Ellipse(int x, int y, int rx, int ry, QColor color);
    void paint(QPixmap *canvas);

    int x;
    int y;
    int rx;
    int ry;
};

#endif // ELLIPSE_H
