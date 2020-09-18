#include "line.h"

Line::Line()
{
    x1 = 0; y1 = 0;
    x2 = 0; y2 = 0;
    color = QColor(0,200,200);
    shape = shapeLine;
    clipOut = false;
}

Line::Line(int x1, int y1, int x2, int y2, QColor color)
{
    this->x1 = x1; this->y1 = y1;
    this->x2 = x2; this->y2 = y2;
    this->color = color;
    shape = shapeLine;
    clipOut = false;
}

void Line::DDA_paint(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);
    //canvasPainter->drawLine(x1,y1, x2, y2);
    int X1 = this->x1;
    int X2 = this->x2;
    int Y1 = this->y1;
    int Y2 = this->y2;
    swap(X1,X2,Y1,Y2);//从左向右处理
    if(X1 == X2)//纵向线段
    {
        //注意：此时Y1Y2未区分大小
        int s = (Y1 <= Y2 ? Y1 : Y2);
        int e = (Y1 <= Y2 ? Y2 : Y1);
        for(int i = s; i <= e; i++)
        {
            canvasPainter->drawPoint(X1, i);
        }
    }
    else if(Y1 == Y2)//横向线段
    {
        //已知X1<X2
        for(int i = X1; i <= X2; i++)
        {
            canvasPainter->drawPoint(i, Y1);
        }
    }
    else
    {
        double m = double(Y2 - Y1) / double(X2 - X1);//m是斜率
        if((m > 0 && m <= 1) || (m < 0 && m >= -1))//斜率绝对值<=1
        {
            double y = Y1;//此时以y作为因变量
            for(int i = X1; i <= X2; i++)
            {
                y += m;
                canvasPainter->drawPoint(i,(int)(round(y)));
            }
        }
        else if(m > 1)//斜率大于1和斜率小于-1要进行区分
        {
            double x = X1;
            double mr = 1.0 / m;
            if(Y1 < Y2)
            {
                for(int i = Y1; i <= Y2; i++)
                {
                    x += mr;
                    canvasPainter->drawPoint((int)(round(x)),i);
                }
            }
            else
            {
                for(int i = Y1; i >= Y2; i--)
                {
                    x += mr;
                    canvasPainter->drawPoint((int)(round(x)),i);
                }
            }
        }
        else//m<-1
        {
            double x = X1;
            double mr = 1.0 / m;
            if(Y1 < Y2)
            {
                for(int i = Y1; i <= Y2; i++)
                {
                    x -= mr;//从左到右处理，此时x应该递增
                    canvasPainter->drawPoint((int)(round(x)),i);
                }
            }
            else
            {
                for(int i = Y1; i >= Y2; i--)
                {
                    x -= mr;
                    canvasPainter->drawPoint((int)(round(x)),i);
                }
            }
        }
    }
    delete canvasPainter;
}

void Line::Bresenham_paint(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    //canvasPainter->setPen(color);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);
    //canvasPainter->drawLine(x1,y1, x2, y2);
    int X1 = this->x1;
    int X2 = this->x2;
    int Y1 = this->y1;
    int Y2 = this->y2;
    swap(X1,X2,Y1,Y2);//从左向右处理
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
    delete canvasPainter;
}
void Line::swap(int& x1, int& x2, int& y1, int& y2)
{
    if(x1 > x2){
        int tmp = x1; x1 = x2; x2 = tmp;
        tmp = y1; y1 = y2; y2 = tmp;
    }
}
QPoint Line::getStart()
{
    return QPoint(x1,y1);
}
QPoint Line::getEnd()
{
    return QPoint(x2,y2);
}
