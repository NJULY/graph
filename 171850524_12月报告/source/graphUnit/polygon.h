#ifndef POLYGON_H
#define POLYGON_H

#include "graphUnit.h"
#include "line.h"

class Polygon : public graphUnit
{
public:
    Polygon();
    Polygon(QColor color);
    void pushLine(Line line);
    void popLine();
    void paint(QPixmap* canvas);
    QPoint getStart();

    QVector<Line> lineVector;
    int algorithm = 0;
};

#endif // POLYGON_H
