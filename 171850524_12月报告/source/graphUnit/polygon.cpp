#include "polygon.h"

Polygon::Polygon()
{
    shape = shapePolygon;
    color = QColor(0,200,200);
}
Polygon::Polygon(QColor color)
{
    this->color = color;
    shape = shapePolygon;
}
void Polygon::pushLine(Line line)
{
    lineVector.push_back(line);
}
void Polygon::popLine()
{
    lineVector.pop_back();
}
void Polygon::paint(QPixmap* canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);

    QVector<Line>::iterator i;
    if(algorithm == 0){
        for(i = lineVector.begin(); i != lineVector.end(); i++)
        {
            //i->Bresenham_paint(canvas);
            QPoint start = i->getStart();
            QPoint end = i->getEnd();
            int X1 = start.x(), Y1 = start.y();
            int X2 = end.x(), Y2 = end.y();
            i->swap(X1,X2,Y1,Y2);
            if(X1 == X2)
            {
                //注意：此时Y1Y2未区分大小
                int s = (Y1 <= Y2 ? Y1 : Y2);
                int e = (Y1 <= Y2 ? Y2 : Y1);
                for(int y = s; y <= e; y++)
                {
                    canvasPainter->drawPoint(X1, y);
                }
            }
            else if(Y1 == Y2)
            {
                //已知X1<X2
                for(int x = X1; x <= X2; x++)
                {
                    canvasPainter->drawPoint(x, Y1);
                }
            }
            else
            {
                double m = double(Y2 - Y1) / double(X2 - X1);
                if(fabs(m - 1) <= 0.0001)
                {
                    int y = Y1;
                    for(int x = X1; x <= X2; x++)
                    {
                        canvasPainter->drawPoint(x,y);
                        y++;
                    }
                }
                else if(fabs(m + 1) <= 0.0001)
                {
                    int y = Y1;
                    for(int x = X1; x <= X2; x++)
                    {
                        canvasPainter->drawPoint(x, y);
                        y--;
                    }
                }
                else if(m > 1) //坐标变换是基于0<m<1时的做法，变换比较复杂，以过x1y1斜率为1的直线进行翻转得到
                {
                    int dx = Y2 - Y1;
                    int dy = X2 - X1;
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int X0 = X1, Y0 = Y1;//x` = y + X0 - Y0; y` = x - X0 + Y0;
                    int y = Y1;
                    for(int x = X1; x <= X1 + Y2 - Y1; x++){
                        canvasPainter->drawPoint(y + X0 - Y0, x - X0 + Y0);
                        if(p >= 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
                else if(m < -1)//最复杂的一种变换,(x,y)水平向下翻折(x,2y0-y)，在沿着-45度翻转(y+x0-y0,-x+x0+y0)
                {
                    int dx = Y1 - Y2;
                    int dy = X2 - X1;
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int X0 = X1, Y0 = Y1;//x` = y + X0 - Y0; y` = x - X0 + Y0;
                    int y = Y1;
                    for(int x = X1; x <= X1 + Y1 - Y2; x++){
                        canvasPainter->drawPoint(y + X0 - Y0, -x + X0 + Y0);
                        if(p >= 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
                else if(m > 0 && m < 1)
                {
                    int dx = X2 - X1;
                    int dy = Y2 - Y1;
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int y = Y1;
                    for(int x = X1; x <= X2; x++){
                        canvasPainter->drawPoint(x,y);
                        if(p > 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
                else// -1<=m<0,通过上一步的情况进行坐标变化得到
                {
                    int dx = X2 - X1; // dx > 0
                    int dy = -(Y2 - Y1); // 迫使dy > 0
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int Y0 = Y1;
                    int y = Y1;
                    //Y1 - Y0 = Y0 - y,可知y=2y0-y1,此为坐标变换
                    for(int x = X1; x <= X2; x++){
                        canvasPainter->drawPoint(x, 2 * Y0 - y);
                        if(p > 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
            }
        }
    }
    else{
        for(i = lineVector.begin(); i != lineVector.end(); i++){
            int X1 = i->x1;
            int X2 = i->x2;
            int Y1 = i->y1;
            int Y2 = i->y2;
            i->swap(X1,X2,Y1,Y2);//从左向右处理
            if(X1 == X2)
            {
                //注意：此时Y1Y2未区分大小
                int s = (Y1 <= Y2 ? Y1 : Y2);
                int e = (Y1 <= Y2 ? Y2 : Y1);
                for(int y = s; y <= e; y++)
                {
                    canvasPainter->drawPoint(X1, y);
                }
            }
            else if(Y1 == Y2)
            {
                //已知X1<X2
                for(int x = X1; x <= X2; x++)
                {
                    canvasPainter->drawPoint(x, Y1);
                }
            }
            else
            {
                double m = double(Y2 - Y1) / double(X2 - X1);
                if(fabs(m - 1) <= 0.0001)
                {
                    int y = Y1;
                    for(int x = X1; x <= X2; x++)
                    {
                        canvasPainter->drawPoint(x,y);
                        y++;
                    }
                }
                else if(fabs(m + 1) <= 0.0001)
                {
                    int y = Y1;
                    for(int x = X1; x <= X2; x++)
                    {
                        canvasPainter->drawPoint(x, y);
                        y--;
                    }
                }
                else if(m > 1) //坐标变换是基于0<m<1时的做法，变换比较复杂，以过x1y1斜率为1的直线进行翻转得到
                {
                    int dx = Y2 - Y1;
                    int dy = X2 - X1;
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int X0 = X1, Y0 = Y1;//x` = y + X0 - Y0; y` = x - X0 + Y0;
                    int y = Y1;
                    for(int x = X1; x <= X1 + Y2 - Y1; x++){
                        canvasPainter->drawPoint(y + X0 - Y0, x - X0 + Y0);
                        if(p >= 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
                else if(m < -1)//最复杂的一种变换,(x,y)水平向下翻折(x,2y0-y)，在沿着-45度翻转(y+x0-y0,-x+x0+y0)
                {
                    int dx = Y1 - Y2;
                    int dy = X2 - X1;
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int X0 = X1, Y0 = Y1;//x` = y + X0 - Y0; y` = x - X0 + Y0;
                    int y = Y1;
                    for(int x = X1; x <= X1 + Y1 - Y2; x++){
                        canvasPainter->drawPoint(y + X0 - Y0, -x + X0 + Y0);
                        if(p >= 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
                else if(m > 0 && m < 1)
                {
                    int dx = X2 - X1;
                    int dy = Y2 - Y1;
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int y = Y1;
                    for(int x = X1; x <= X2; x++){
                        canvasPainter->drawPoint(x,y);
                        if(p > 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
                else// -1<=m<0,通过上一步的情况进行坐标变化得到
                {
                    int dx = X2 - X1; // dx > 0
                    int dy = -(Y2 - Y1); // 迫使dy > 0
                    int dydy = 2 * dy;
                    int dydy_dxdx = 2 * (dy - dx);
                    int p = 2 * dy - dx;
                    int Y0 = Y1;
                    int y = Y1;
                    //Y1 - Y0 = Y0 - y,可知y=2y0-y1,此为坐标变换
                    for(int x = X1; x <= X2; x++){
                        canvasPainter->drawPoint(x, 2 * Y0 - y);
                        if(p > 0)
                        {
                            p += dydy_dxdx;
                            y++;
                        }
                        else
                        {
                            p += dydy;
                        }
                    }
                }
            }
        }
    }
    delete canvasPainter;
}
QPoint Polygon::getStart()
{
    if(lineVector.isEmpty() == false)
        return lineVector[0].getStart();
    else
        return QPoint(-100,-100);
}
