#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <QPoint>
#include <QColor>
#include <QVector>
#include <QPixmap>
#include "graphUnit/graphUnit.h"
#include "graphUnit/line.h"
#include "graphUnit/circle.h"
#include "graphUnit/ellipse.h"
#include "graphUnit/polygon.h"
#include "graphUnit/curve.h"

class Transformation
{
public:
    Transformation();
    static void translate(graphUnit* unit, int dx, int dy);
    static void rotate(graphUnit* unit, int x, int y, double r);
    static void scale(graphUnit* unit, int x, int y, double s);
    static void clipLiangBarsky(QPoint point1, QPoint point2, Line* line);
    static void clipCohenSutherland(QPoint point1, QPoint point2, Line* line);
};

#endif // TRANSFORMATION_H
