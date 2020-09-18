#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QPoint>
#include <QColor>
#include <QVector>
#include <QPixmap>
#include <QtMath>
#include "graphUnit/graphUnit.h"
#include "graphUnit/line.h"
#include "graphUnit/circle.h"
#include "graphUnit/ellipse.h"
#include "graphUnit/polygon.h"
#include "graphUnit/rectangle.h"
#include "graphUnit/curve.h"

#define CANVAS_W 600
#define CANVAS_H 600
#define PI 3.14159265358

enum Command {cmdFree, cmdDrawLine, cmdDrawCircle, cmdDrawEllipse,
              cmdDrawPolygonStart, cmdDrawPolygonIng,
              cmdDrawCurveStart, cmdDrawCurveIng
             };

class Controller
{
public:
    Controller();
    void addGraph(QPoint startPoint, QPoint endPoint, QColor color);
    void removeGraph();
    void reset();//重置画布
    void paint(QPixmap* canvas);//绘制图元
    //辅助图形变换的函数
    static void choseGraph(graphUnit *unit, int x, int y);//判断鼠标是否选中当前图元
    static double getAngle(int x1, int y1, int x2, int y2);//计算与x轴正向的弧度
    graphUnit* getPointer(int id);

    int lineAlgorithm;
    int curveAlgorithm;
    int clipAlgorithm;
    int polygonAlgorithm;
    Command cmd;
    QVector<graphUnit*> graphs;//存储所有的图元
    graphUnit* unit;
};

#endif // CONTROLLER_H
