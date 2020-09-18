#ifndef GRAPHUNIT_H
#define GRAPHUNIT_H
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <cmath>
#include <QVector>
enum Shape {unKnown, shapeLine, shapeCircle, shapeEllipse, shapePolygon,
           shapeRectangle, shapeCurve
           };

class graphUnit
{
public:
    graphUnit();
    Shape getShape();

    bool beChosen;//区分图元是否被选中
    Shape shape;//图元的形状
    int id;//图元id
    QColor color;//图元颜色

    //用于记录平移时鼠标位置
    int lastX;
    int lastY;
    //用于记录旋转时鼠标位置
    int rotateX;
    int rotateY;
    //用于记录缩放时鼠标位置
    int scaleX;
    int scaleY;
};

#endif
