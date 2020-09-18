#include "circle.h"

Circle::Circle()
{
    x = 0;
    y = 0;
    r = 0;
    shape = shapeCircle;
    color = QColor(0,200,200);
}

Circle::Circle(int x, int y, int r, QColor color)
{
    this->x = x;
    this->y = y;
    this->r = r;
    shape = shapeCircle;
    this->color = color;
}

void Circle::paint(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);
    int X = 0;
    int Y = r;
    //double p = 1.25 - r;
    int p = 5 - 4*r;
    while(X <= Y)
    {
        //画八个点
        canvasPainter->drawPoint(X+x,Y+y);
        canvasPainter->drawPoint(X+x,-Y+y);
        canvasPainter->drawPoint(-X+x,Y+y);
        canvasPainter->drawPoint(-X+x,-Y+y);
        canvasPainter->drawPoint(Y+x,X+y);
        canvasPainter->drawPoint(Y+x,-X+y);
        canvasPainter->drawPoint(-Y+x,X+y);
        canvasPainter->drawPoint(-Y+x,-X+y);
        //更新XY
        X++;
        if(p < 0)
        {
            p += 4*(2 * X + 1);
        }
        else
        {
            Y--;
            p += 4*(2 * X + 1 - 2 * Y);
        }
    }

    delete canvasPainter;
}
