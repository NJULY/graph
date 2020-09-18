#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "graphUnit.h"

class Rectangle : public graphUnit
{
public:
    Rectangle();
    Rectangle(int x1, int y1, int x2, int y2, QColor color);
    void paint(QPixmap *canvas);
    int x1, y1, x2, y2;
};

#endif // RECTANGLE_H
