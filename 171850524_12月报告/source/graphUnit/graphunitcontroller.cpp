#include "graphUnitController.h"

GraphUnitController::GraphUnitController()
{

}

void GraphUnitController::addGraph(QPoint startPoint, QPoint endPoint, QColor color, Command cmd)
{
    if(cmd == cmdDrawLine)//画直线，鼠标开始和结束分别对应着直线端点
    {
        int x1, y1, x2, y2;
        x1 = startPoint.x() - OFF_X;
        y1 = startPoint.y() - OFF_Y;
        x2 = endPoint.x() - OFF_X;
        y2 = endPoint.y() - OFF_Y;
        unit = new Line(x1, y1, x2, y2, color);
        graphs.push_back(unit);
    }
    else if(cmd == cmdDrawCircle)//画圆，鼠标点击为圆心，移动中计算半径
    {
        int x, y, r;
        int rX, rY;
        x = startPoint.x() - OFF_X;
        y = startPoint.y() - OFF_Y;
        rX = endPoint.x() - OFF_X;
        rY = endPoint.y() - OFF_Y;
        r = (int)sqrt(abs(rX - x)*abs(rX - x)+abs(rY - y)*abs(rY - y));
        unit = new Circle(x, y, r, color);
        graphs.push_back(unit);
    }
    else if(cmd == cmdDrawEllipse)//画椭圆，鼠标起点为椭圆中心，终点与起点可计算两个轴
    {
        int x, y, rx, ry;
        int sx, sy, ex, ey;
        sx = startPoint.x() - OFF_X;
        sy = startPoint.y() - OFF_Y;
        ex = endPoint.x() - OFF_X;
        ey = endPoint.y() - OFF_Y;
        x = sx;
        y = sy;
        rx = abs(sx-ex);
        ry = abs(sy-ey);
        unit = new Ellipse(x, y, rx, ry, color);
        graphs.push_back(unit);
    }
}

void GraphUnitController::removeGraph()
{
    if(graphs.isEmpty() == false)
    {
        graphs.pop_back();
    }
}
