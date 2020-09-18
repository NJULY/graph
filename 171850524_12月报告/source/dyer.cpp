#include "dyer.h"

Dyer::Dyer()
{

}

void Dyer::dyeGraph(graphUnit* unit, QPixmap* canvas)
{
    Shape shape = unit->shape;
    QPainter* canvasPainter=new QPainter(canvas);
    QPoint* points = new QPoint[1000000];
    int pointsLen = 0;
    //采用扫描边界转换算法
    if(shape == shapePolygon)//多边形染色
    {
        Polygon* polygon = (Polygon*)unit;
        int size = polygon->lineVector.size();
        int maxY = 0, minY = 1001;
        for(int i = 0; i < size; i++)
        {
            //printf("(%d,%d)--",polygon->lineVector[i].x1,polygon->lineVector[i].y1);
            int j = (i+1) % size;
            //找出扫描线的最大值
            if(polygon->lineVector[i].y1 > maxY)
                maxY = polygon->lineVector[i].y1;
            if(polygon->lineVector[i].y2 > maxY)
                maxY = polygon->lineVector[i].y1;
            if(polygon->lineVector[j].y1 > maxY)
                maxY = polygon->lineVector[i].y1;
            if(polygon->lineVector[j].y2 > maxY)
                maxY = polygon->lineVector[i].y1;
            //找出扫描线的最小值
            if(polygon->lineVector[i].y1 < minY)
                minY = polygon->lineVector[i].y1;
            if(polygon->lineVector[i].y2 < minY)
                minY = polygon->lineVector[i].y1;
            if(polygon->lineVector[j].y1 < minY)
                minY = polygon->lineVector[i].y1;
            if(polygon->lineVector[j].y2 < minY)
                minY = polygon->lineVector[i].y1;
            //对顶点进行处理
            if(polygon->lineVector[i].y1 > polygon->lineVector[i].y2
                    && polygon->lineVector[j].y1 > polygon->lineVector[j].y2
                    && polygon->lineVector[i].y2 == polygon->lineVector[j].y1)
                polygon->lineVector[j].y1 -= 1;
            else if(polygon->lineVector[i].y1 < polygon->lineVector[i].y2
                    && polygon->lineVector[j].y1 < polygon->lineVector[j].y2
                    && polygon->lineVector[i].y2 == polygon->lineVector[j].y1)
                polygon->lineVector[i].y2 -= 1;
        }
        //使用扫描线求交点
        for(int i = minY; i <= maxY; i++)
        {
            int *pointX = new int[1000];
            int len = 0;
            for(int j = 0; j< size; j++)
            {
                int x1 = polygon->lineVector[j].x1;
                int x2 = polygon->lineVector[j].x2;
                int y1 = polygon->lineVector[j].y1;
                int y2 = polygon->lineVector[j].y2;
                //printf("(%d,%d),(%d,%d)\n",x1,y1,x2,y2);
                if(y1 == y2)
                {
                    continue;
                }
                else if(y1 > y2)
                {
                    int tmp = x1; x1 = x2; x2 = tmp;
                    tmp = y1; y1 = y2; y2 = tmp;
                }
                if(y1 > i || y2 < i)
                    continue;

                int x = double(i - y1)*(double)(x2-x1)/(double)(y2-y1) + x1;
                pointX[len] = x; len++;
            }
            std::sort(pointX, pointX+len);//排序
            if(len % 2 != 0)
            {
                printf("odd intersection\n");
                return;
            }
            for(int j = 0; j < len; j+=2)
            {
                for(int k = pointX[j]; k<= pointX[j+1]; k++)
                {
                    points[pointsLen] = QPoint(k,i);
                    pointsLen++;
                }
            }
            delete []pointX;
        }
        canvasPainter->drawPoints(points, pointsLen);
    }
    else//采用区域填充-扫描边界填充
    {
        //其他图形包括圆、椭圆，填充较为简单
        if(shape == shapeCircle)
        {
            Circle* circle = (Circle*)unit;
            int x = circle->x;
            int y = circle->y;
            int r = circle->r;
            for(int i = x-r; i <= x+r; i++)
            {
                for(int j = y-r; j <= y+r; j++)
                {
                    if((i-x)*(i-x)+(j-y)*(j-y) < r*r)
                    {
                        points[pointsLen] = QPoint(i,j);
                        pointsLen++;
                    }
                }
            }
        }
        else if(shape == shapeEllipse)
        {
            Ellipse* ellipse = (Ellipse*)unit;
            int x = ellipse->x;
            int y = ellipse->y;
            int rx = ellipse->rx;
            int ry = ellipse->ry;
            for(int i = x-rx; i <= x+rx; i++)
            {
                for(int j = y-ry; j <= y+ry; j++)
                {
                    if(ry*ry*(i-x)*(i-x) + rx*rx*(j-y)*(j-y) < rx*rx*ry*ry)
                    {
                        points[pointsLen] = QPoint(i,j);
                        pointsLen++;
                    }
                }
            }
        }
        canvasPainter->drawPoints(points, pointsLen);
    }
    delete []points;
    delete canvasPainter;
}

bool Dyer::inOutTest(graphUnit* unit, QPoint p)
{
    //设置一条从p出发，水平向右的射线
    //若交点个数为奇数，则内部，否则外部
    Shape shape = unit->getShape();
    int x = p.x(), y = p.y();
    if(shape == shapePolygon)
    {
        Polygon* polygon = (Polygon*)unit;
        int nCross = 0;
        QVector<Line>::iterator i;
        for(i = polygon->lineVector.begin(); i != polygon->lineVector.end(); i++)
        {
            Line tmpLine = *i;
            //printf("x:%d,y:%d ",x,y);
            //printf("(%d,%d),(%d,%d) ",tmpLine.x1,tmpLine.y1,tmpLine.x2,tmpLine.y2);
            if(tmpLine.y1 == tmpLine.y2)
                continue;
            if(y < ((tmpLine.y1 < tmpLine.y2)?tmpLine.y1:tmpLine.y2))
            {
                continue;
            }
            if(y > ((tmpLine.y1 > tmpLine.y2)?tmpLine.y1:tmpLine.y2))
                continue;
            //计算线段在p所处的y值对应的x值
            double Rm = (double)(tmpLine.x2 - tmpLine.x1) / (double)(tmpLine.y2-tmpLine.y1);
            double xLine = double(y - tmpLine.y1) * Rm + tmpLine.x1;
            if(xLine > x)
                nCross++;
            //printf("x:%d,x`:%d ",x,(int)xLine);
        }
        //printf("==nCross:%d==",nCross);
        if(nCross % 2 == 1)
            return true;
        else
            return false;
    }
    else if(shape == shapeCircle)
    {
        Circle* circle = (Circle*)unit;
        int distance = (x-circle->x)*(x-circle->x)+(y-circle->y)*(y-circle->y);
        if(distance < circle->r*circle->r)
            return true;
        else
            return false;
    }
    else if(shape == shapeEllipse)
    {
        Ellipse* ellipse = (Ellipse*) unit;
        x -= ellipse->x;
        y -= ellipse->y;
        if(ellipse->ry*ellipse->ry*x*x + ellipse->rx*ellipse->rx*y*y < ellipse->rx*ellipse->rx*ellipse->ry*ellipse->ry)
            return true;
        else
            return false;
    }
    return false;
}
