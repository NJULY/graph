#ifndef DYER_H
#define DYER_H

#include <QPoint>
#include <QColor>
#include <QVector>
#include <QPixmap>
#include "graphUnit/graphUnit.h"
#include "graphUnit/line.h"
#include "graphUnit/circle.h"
#include "graphUnit/ellipse.h"
#include "graphUnit/polygon.h"
#include <cmath>

class Dyer
{
public:
    Dyer();
    static bool inOutTest(graphUnit* unit, QPoint p);//内外点测试
    static void dyeGraph(graphUnit* unit, QPixmap* canvas);//填充算法
};

#endif // DYER_H
