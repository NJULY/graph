#ifndef LINE_H
#define LINE_H
#include "graphUnit.h"

class Line : public graphUnit
{
public:
    Line();
    Line(int x1, int y1, int x2, int y2, QColor color);
    void DDA_paint(QPixmap *canvas);
    void Bresenham_paint(QPixmap *canvas);
    QPoint getStart();
    QPoint getEnd();
    void swap(int& x1, int& x2, int& y1, int& y2);

    int x1,y1,x2,y2;
    bool clipOut;
    int lineAlgorithm=0;
};

#endif // LINE_H
