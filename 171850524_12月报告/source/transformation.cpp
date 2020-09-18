#include "transformation.h"

Transformation::Transformation(){}

void Transformation::translate(graphUnit* unit, int dx, int dy)
{
    Shape shape = unit->shape;
    if(shape == shapeLine)
    {
        Line* line = (Line*)unit;
        line->x1 += dx;
        line->x2 += dx;
        line->y1 += dy;
        line->y2 += dy;
    }
    else if(shape == shapeCircle)
    {
        Circle* circle = (Circle*)unit;
        circle->x += dx;
        circle->y += dy;
    }
    else if(shape == shapeEllipse)
    {
        Ellipse* ellipse = (Ellipse*)unit;
        ellipse->x += dx;
        ellipse->y += dy;
    }
    else if(shape == shapePolygon)
    {
        Polygon* polygon = (Polygon*)unit;
        QVector<Line>::iterator i;
        for(i = polygon->lineVector.begin(); i != polygon->lineVector.end(); i++)
        {
            i->x1 += dx;
            i->y1 += dy;
            i->x2 += dx;
            i->y2 += dy;
        }
    }
    else if(shape == shapeCurve){
        Curve *curve = (Curve*)unit;
        int size = curve->pointVector.size();
        for(int i = 0; i< size; i++){
            curve->pointVector[i]+=QPoint(dx,dy);
        }
    }
}

void Transformation::rotate(graphUnit* unit, int x, int y, double r)
{
    //r采用弧度制，输入时需要提前做转换
    Shape shape = unit->shape;
    if(shape == shapeLine){
        Line* line = (Line*)unit;
        int x1 = line->x1;
        int x2 = line->x2;
        int y1 = line->y1;
        int y2 = line->y2;
        line->x1 = (int)(x + (x1-x)*cos(r)-(y1-y)*sin(r));
        line->y1 = (int)(y + (x1-x)*sin(r)+(y1-y)*cos(r));
        line->x2 = (int)(x + (x2-x)*cos(r)-(y2-y)*sin(r));
        line->y2 = (int)(y + (x2-x)*sin(r)+(y2-y)*cos(r));
    }
    else if(shape == shapeCircle){
        Circle* circle = (Circle*)unit;
        int x1 = circle->x;
        int y1 = circle->y;
        circle->x = (int)(x + (x1-x)*cos(r) - (y1-y)*sin(r));
        circle->y = (int)(y + (x1-x)*sin(r) + (y1-y)*cos(r));
    }
    else if(shape == shapeEllipse){
        Ellipse* ellipse = (Ellipse*)unit;
        int x1 = ellipse->x;
        int y1 = ellipse->y;
        ellipse->x = (int)(x + (x1-x)*cos(r) - (y1-y)*sin(r));
        ellipse->y = (int)(y + (x1-x)*sin(r) + (y1-y)*cos(r));
    }
    else if(shape == shapePolygon){
        Polygon* polygon = (Polygon*)unit;
        int len = polygon->lineVector.size();
        for(int i = 0; i < len; i++){
            rotate(&(polygon->lineVector[i]),x,y,r);
        }
    }
    else if(shape == shapeCurve){
        Curve* curve = (Curve*)unit;
        int size = curve->pointVector.size();
        for(int i = 0; i < size; i++){
            int x1 = curve->pointVector[i].x();
            int y1 = curve->pointVector[i].y();
            int resX = (int)(x + (x1-x)*cos(r)-(y1-y)*sin(r));
            int resY = (int)(y + (x1-x)*sin(r)+(y1-y)*cos(r));
            curve->pointVector[i] = QPoint(resX, resY);
        }
    }
}

void Transformation::scale(graphUnit* unit, int x, int y, double s)
{
    Shape shape = unit->shape;
    if(shape == shapeLine)
    {
        Line* line = (Line*)unit;
        int x1 = line->x1;
        int x2 = line->x2;
        int y1 = line->y1;
        int y2 = line->y2;
        line->x1 = (int)((x1-x)*s) + x;
        line->x2 = (int)((x2-x)*s) + x;
        line->y1 = (int)((y1-y)*s) + y;
        line->y2 = (int)((y2-y)*s) + y;
    }
    else if(shape == shapeCircle)
    {
        Circle* circle = (Circle*)unit;
        int x1 = circle->x;
        int y1 = circle->y;
        circle->x = (int)((x1-x)*s) + x;
        circle->y = (int)((y1-y)*s) + y;
        circle->r = (int)(circle->r*s);
    }
    else if(shape == shapeEllipse)
    {
        Ellipse* ellipse = (Ellipse*)unit;
        int x1 = ellipse->x;
        int y1 = ellipse->y;
        ellipse->x = (int)((x1-x)*s) + x;
        ellipse->y = (int)((y1-y)*s) + y;
        ellipse->rx = (int)(ellipse->rx*s);
        ellipse->ry = (int)(ellipse->ry*s);
    }
    else if(shape == shapePolygon)
    {
        Polygon* polygon = (Polygon*)unit;
        int len = polygon->lineVector.size();
        for(int i =0; i< len; i++)
        {
            scale(&(polygon->lineVector[i]),x,y,s);
        }
    }
    else if(shape == shapeCurve){
        Curve* curve = (Curve*)unit;
        int size = curve->pointVector.size();
        for(int i = 0; i < size; i++) {
            int x1 = curve->pointVector[i].x();
            int y1 = curve->pointVector[i].y();
            int x2 = (int)((x1-x)*s) + x;
            int y2 = (int)((y1-y)*s) + y;
            curve->pointVector[i] = QPoint(x2, y2);
        }
    }
    else
    {
        printf("unknown shape in function scale\n");
    }
}

void Transformation::clipLiangBarsky(QPoint point1, QPoint point2, Line* line)
{
    if(line == nullptr)
        return;
    //首先得到窗口坐标
    int xmin = point1.x()>point2.x()?point2.x():point1.x();
    int xmax = point1.x()>point2.x()?point1.x():point2.x();
    int ymin = point1.y()>point2.y()?point2.y():point1.y();
    int ymax = point1.y()>point2.y()?point1.y():point2.y();
    //计算pi,qi
    int x1 = line->x1, x2 = line->x2, y1 = line->y1, y2 = line->y2;
    int p1 = x1 - x2, q1 = x1 - xmin;
    int p2 = x2 - x1, q2 = xmax - x1;
    int p3 = y1 - y2, q3 = y1 - ymin;
    int p4 = y2 - y1, q4 = ymax - y1;
    //检测直线平行于裁剪窗口的情况
    if(p1 == 0 && (q1 < 0 || q2 < 0))
    {
        line->clipOut = true;
        return;//平行于y轴且在窗口外
    }
    if(p3 == 0 && (q3 < 0 || q4 < 0))
    {
        line->clipOut = true;
        return;//平行于x轴且在窗口外
    }
    if(p1 == 0)//平行于y轴
    {
        if(y1 < ymin && y2 < ymin)
        {
            line->clipOut = true;
            return;
        }
        if(y1 > ymax && y2 > ymax)
        {
            line->clipOut = true;
            return;
        }
        if(y1 <= ymax && y2 <= ymax && y1 >= ymin && y2 >= ymin)
            return;
        //此时端点至少有一个在外侧
        if(y1 <= ymax && y1 >= ymin)//y2在窗口外
        {
           if(y2 > ymax)
               line->y2 = ymax;
           else
               line->y2 = ymin;
        }
        else if(y2 <= ymax && y2 >= ymin)//y1在窗口外
        {
           if(y1 > ymax)
               line->y1 = ymax;
           else
               line->y1 = ymin;
        }
        else//两个端点都在外侧
        {
            line->y1 = ymin;
            line->y2 = ymax;
        }
        return;
    }
    if(p3 == 0)//平行于x轴
    {
        if(x1 < xmin && x2 < xmin)
        {
            line->clipOut = true;
            return;
        }
        if(x1 > xmax && x2 > xmax)
        {
            line->clipOut = true;
            return;
        }
        if(x1 <= xmax && x2 <= xmax && x1 >= xmin && x2 >= xmin)
            return;
        //此时端点至少有一个在外侧
        if(x1 <= xmax && x1 >= xmin)//x2在窗口外
        {
           if(x2 > xmax)
               line->x2 = xmax;
           else
               line->x2 = xmin;
        }
        else if(x2 <= xmax && x2 >= xmin)//x1在窗口外
        {
           if(x1 > xmax)
               line->x1 = xmax;
           else
               line->x1 = xmin;
        }
        else//两个端点都在外侧
        {
            line->x1 = xmin;
            line->x2 = xmax;
        }
        return;
    }
    //计算u1与u2
    double u1 = 0.0, u2 = 1.0;
    double r1 = (double)q1 / (double)p1;
    double r2 = (double)q2 / (double)p2;
    double r3 = (double)q3 / (double)p3;
    double r4 = (double)q4 / (double)p4;
    if(p1 < 0 && p3 < 0)//p1 p3 < 0; p2 p4 >0;
    {
        if(r1 > u1)
            u1 = r1;
        if(r3 > u1)
            u1 = r3;
        if(r2 < u2)
            u2 = r2;
        if(r4 < u2)
            u2 = r4;
    }
    else if(p1 < 0 && p3 > 0)//p1 p4 < 0; p2 p3 > 0;
    {
        if(r1 > u1)
            u1 = r1;
        if(r4 > u1)
            u1 = r4;
        if(r2 < u2)
            u2 = r2;
        if(r3 < u2)
            u2 = r3;
    }
    else if(p1 > 0 && p3 < 0)//p2 p3 <0; p1 p4 > 0;
    {
        if(r2 > u1)
            u1 = r2;
        if(r3 > u1)
            u1 = r3;
        if(r1 < u2)
            u2 = r1;
        if(r4 < u2)
            u2 = r4;
    }
    else if(p1 >0 && p3 > 0)//p2 p4 < 0; p1 p3 > 0;
    {
        if(r2 > u1)
            u1 = r2;
        if(r4 > u1)
            u1 = r4;
        if(r1 < u2)
            u2 = r1;
        if(r3 < u2)
            u2 = r3;
    }
    else
    {
        printf("wrong in liang-barsky\n");
    }
    //对u1u2进行判断，计算裁剪后线段端点
    if(u1 > u2)//线段在裁剪窗口外   
    {
        line->clipOut = true;
        return;
    }
    double resX1 = (double)x1 + u1*(double)(x2 - x1);
    double resY1 = (double)y1 + u1*(double)(y2 - y1);
    double resX2 = (double)x1 + u2*(double)(x2 - x1);
    double resY2 = (double)y1 + u2*(double)(y2 - y1);
    //对裁剪后的端点进行四舍五入的选择
    if(resX1 < (int)resX1 + 0.5)
        line->x1 = (int)resX1;
    else
        line->x1 = (int)resX1 + 1;
    if(resY1 < (int)resY1 + 0.5)
        line->y1 = (int)resY1;
    else
        line->y1 = (int)resY1 + 1;
    if(resX2 < (int)resX2 + 0.5)
        line->x2 = (int)resX2;
    else
        line->x2 = (int)resX2 + 1;
    if(resY2 < (int)resY2 + 0.5)
        line->y2 = (int)resY2;
    else
        line->y2 = (int)resY2 + 1;
}

void Transformation::clipCohenSutherland(QPoint point1, QPoint point2, Line* line)
{
    if(line == nullptr)
        return;
    //首先得到窗口坐标
    int xmin = point1.x()>point2.x()?point2.x():point1.x();
    int xmax = point1.x()>point2.x()?point1.x():point2.x();
    int ymin = point1.y()>point2.y()?point2.y():point1.y();
    int ymax = point1.y()>point2.y()?point1.y():point2.y();
    //计算两个端点的区域码
    int x1 = line->x1, x2 = line->x2, y1 = line->y1, y2 = line->y2;
    int code1[4], code2[4];
    if(y1 > ymax)
        code1[0] = 1;
    else
        code1[0] = 0;
    if(y1 < ymin)
        code1[1] = 1;
    else
        code1[1] = 0;
    if(x1 > xmax)
        code1[2] = 1;
    else
        code1[2] = 0;
    if(x1 < xmin)
        code1[3] = 1;
    else
        code1[3] = 0;

    if(y2 > ymax)
        code2[0] = 1;
    else
        code2[0] = 0;
    if(y2 < ymin)
        code2[1] = 1;
    else
        code2[1] = 0;
    if(x2 > xmax)
        code2[2] = 1;
    else
        code2[2] = 0;
    if(x2 < xmin)
        code2[3] = 1;
    else
        code2[3] = 0;
    //对区域码进行判断
    if(code1[0] == 0 && code1[1] == 0 && code1[2] == 0 && code1[3] == 0
            && code2[0] == 0 && code2[1] == 0 && code2[2] == 0 && code2[3] == 0)
        return;//在窗口内
    if(code1[0] == code2[0] && code1[0] == 1)//在窗口上方
    {
        line->clipOut = true;
        return;
    }
    if(code1[1] == code2[1] && code1[1] == 1)//在窗口下方
    {
        line->clipOut = true;
        return;
    }
    if(code1[2] == code2[2] && code1[2] == 1)//在窗口左方
    {
        line->clipOut = true;
        return;
    }
    if(code1[3] == code2[3] && code1[3] == 1)//在窗口右方
    {
        line->clipOut = true;
        return;
    }
    //不能判断的情况，进行求交处理
    if(x1 == x2)//平行于y轴
    {//此时端点至少有一个在外侧
        if(y1 <= ymax && y1 >= ymin)//y2在窗口外
        {
            if(y2 > ymax)
                line->y2 = ymax;
            else
                line->y2 = ymin;
        }
        else if(y2 <= ymax && y2 >= ymin)//y1在窗口外
        {
            if(y1 > ymax)
                line->y1 = ymax;
            else
                line->y1 = ymin;
        }
        else//两个端点都在外侧
        {
            line->y1 = ymin;
            line->y2 = ymax;
        }
        return;
    }
    if(y1 == y2)//平行于x轴
    {
        if(x1 <= xmax && x1 >= xmin)//x2在窗口外
        {
            if(x2 > xmax)
                line->x2 = xmax;
            else
                line->x2 = xmin;
        }
        else if(x2 <= xmax && x2 >= xmin)//x1在窗口外
        {
            if(x1 > xmax)
                line->x1 = xmax;
            else
                line->x1 = xmin;
        }
        else//两个端点都在外侧
        {
            line->x1 = xmin;
            line->x2 = xmax;
        }
        return;
    }
    double deltaX = (double)(x2 - x1), deltaY = (double)(y2 - y1);
    int res[4], cnt = 0;
    res[0]=0;res[1]=0;res[2]=0;res[3]=0;
    //计算与ymax的交点
    double tmpX = (double)(ymax - y1)*deltaX/deltaY + x1;
    tmpX = round(tmpX);
    if(tmpX <= xmax && tmpX >= xmin)
    {
        res[cnt] = (int)tmpX;
        res[cnt+1] = ymax;
        cnt += 2;
    }
    //计算与ymin的交点
    tmpX = (double)(ymin - y1)*deltaX/deltaY + x1;
    tmpX = round(tmpX);
    if(tmpX <= xmax && tmpX >= xmin)
    {
        res[cnt] = (int)tmpX;
        res[cnt+1] = ymin;
        cnt += 2;
    }
    //计算与xmin的交点
    double tmpY = (double)(xmin - x1)*deltaY/deltaX + y1;
    tmpY = round(tmpY);
    if(tmpY >= ymin && tmpY <= ymax)
    {
        if(cnt >= 4)
        {
            printf("wrong in Cohen-sutherland\n");
            return;
        }
        res[cnt] = xmin;
        res[cnt + 1] = (int)tmpY;
        cnt += 2;
    }
    //计算与xmax的交点
    tmpY = (double)(xmax - x1)*deltaY/deltaX + y1;
    tmpY = round(tmpY);
    if(tmpY >= ymin && tmpY <= ymax)
    {
        if(cnt >= 4)
        {
            printf("wrong in Cohen-sutherland\n");
            return;
        }
        res[cnt] = xmax;
        res[cnt + 1] = (int)tmpY;
        cnt += 2;
    }
    if(cnt == 0)//线段完全不在窗口内
    {
        line->clipOut = true;
        printf("out");
        return;
    }
    if((x1 < xmin || x1 > xmax || y1 < ymin || y1 > ymax)
            && (x2 < xmin || x2 > xmax || y2 < ymin || y2 > ymax)){//两个端点都在窗口外
        line->x1 = res[0];
        line->y1 = res[1];
        line->x2 = res[2];
        line->y2 = res[3];
    }
    else if(x1 < xmin || x1 > xmax || y1 < ymin || y1 > ymax)//第一个端点在窗口外
    {
        if((res[0] <= x1 && res[0] >= x2) || (res[0] <= x2 && res[0] >= x1))
        {
            line->x1 = res[0];
            line->y1 = res[1];
        }
        else
        {
            line->x1 = res[2];
            line->y1 = res[3];
        }
    }
    else if(x2 < xmin || x2 > xmax || y2 < ymin || y2 > ymax)//第二个端点在窗口外
    {
        if((res[0] <= x1 && res[0] >= x2) || (res[0] <= x2 && res[0] >= x1))
        {
            line->x2 = res[0];
            line->y2 = res[1];
        }
        else
        {
            line->x2 = res[2];
            line->y2 = res[3];
        }
    }
}
