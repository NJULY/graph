#ifndef CIRCLE_H
#define CIRCLE_H

#include "graphUnit.h"

class Circle : public graphUnit
{
public:
    Circle();
    Circle(int x, int y, int r, QColor color);
    void paint(QPixmap *canvas);
    int x;
    int y;
    int r;
};

#endif // CIRCLE_H
