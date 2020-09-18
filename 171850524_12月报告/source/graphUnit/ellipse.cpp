#include "ellipse.h"

Ellipse::Ellipse()
{
    x = 0;
    y = 0;
    rx = 0;
    ry = 0;
    shape = shapeEllipse;
    color = QColor(0,200,200);
}

Ellipse::Ellipse(int x, int y, int rx, int ry, QColor color)
{
    this->x = x;
    this->y = y;
    this->rx = rx;
    this->ry = ry;
    this->color = color;
    shape = shapeEllipse;
}

void Ellipse::paint(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);
    //算法开始
    int X = 0;
    int Y = ry;
    //将p变为原来的4倍，转成整数，加快运算
    int p = 4*(ry * ry) - 4*(rx * rx * ry) + (rx * rx);
    //第一部分
    while(ry*ry*X < rx*rx*Y)//一二区域分界条件
    {
        //根据对称性，画四个点
        canvasPainter->drawPoint(X+x, Y+y);
        canvasPainter->drawPoint(-X+x, Y+y);
        canvasPainter->drawPoint(X+x, -Y+y);
        canvasPainter->drawPoint(-X+x, -Y+y);
        X++;
        if(p < 0)//选yk
        {
            p += 4*(2*ry*ry*X + ry*ry);
        }
        else//选yk-1,需要进行Y--操作
        {
            Y--;
            p += 4*(2*ry*ry*X - 2*rx*rx*Y + ry*ry);
        }
    }
    //第二部分
    //p = ry*ry*(X+0.5)*(X+0.5) + rx*rx*(Y-1)*(Y-1) - rx*rx*ry*ry;
    p = ry*ry*(2*X+1)*(2*X+1) + 4*rx*rx*(Y-1)*(Y-1) - 4*rx*rx*ry*ry;
    while(Y >= 0)//画到x轴即可
    {
        //根据对称性，画四个点
        canvasPainter->drawPoint(X+x, Y+y);
        canvasPainter->drawPoint(-X+x, Y+y);
        canvasPainter->drawPoint(X+x, -Y+y);
        canvasPainter->drawPoint(-X+x, -Y+y);
        Y--;
        if(p < 0)//选xk+1
        {
            X++;
            p += 4*(2*ry*ry*X - 2*rx*rx*Y + rx*rx);
        }
        else
        {
            p += 4*(-2*rx*rx*Y + rx*rx);
        }
    }
    //算法结束
    delete canvasPainter;
}
