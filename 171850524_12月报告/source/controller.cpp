#include "controller.h"


Controller::Controller()
{
    unit = nullptr;
    cmd = cmdFree;
    lineAlgorithm = 0;
    curveAlgorithm = 0;
    clipAlgorithm = 0;
    polygonAlgorithm = 0;
}

void Controller::addGraph(QPoint startPoint, QPoint endPoint, QColor color)
{
    if(cmd == cmdDrawLine)//画直线，鼠标开始和结束分别对应着直线端点
    {
        int x1, y1, x2, y2;
        x1 = startPoint.x();
        y1 = startPoint.y();
        x2 = endPoint.x();
        y2 = endPoint.y();
        Line* line = new Line(x1, y1, x2, y2, color);
        line->lineAlgorithm = lineAlgorithm;
        graphs.push_back(line);
    }
    else if(cmd == cmdDrawCircle)//画圆，鼠标点击为圆心，移动中计算半径
    {
        int x, y, r;
        int rX, rY;
        x = startPoint.x();
        y = startPoint.y();
        rX = endPoint.x();
        rY = endPoint.y();
        r = (int)sqrt(abs(rX - x)*abs(rX - x)+abs(rY - y)*abs(rY - y));
        unit = new Circle(x, y, r, color);
        graphs.push_back(unit);
    }
    else if(cmd == cmdDrawEllipse)//画椭圆，鼠标起点为椭圆中心，终点与起点可计算两个轴
    {
        int x, y, rx, ry;
        int sx, sy, ex, ey;
        sx = startPoint.x();
        sy = startPoint.y();
        ex = endPoint.x();
        ey = endPoint.y();
        x = sx;
        y = sy;
        rx = abs(sx-ex);
        ry = abs(sy-ey);
        unit = new Ellipse(x, y, rx, ry, color);
        graphs.push_back(unit);
    }
    else if(cmd == cmdDrawPolygonStart){
        int x1, y1, x2, y2;
        x1 = startPoint.x();
        y1 = startPoint.y();
        x2 = endPoint.x();
        y2 = endPoint.y();
        Line* line = new Line(x1, y1, x2, y2, color);
        Polygon* polygon = new Polygon(color);
        polygon->pushLine(*line);
        delete line;
        polygon->algorithm = polygonAlgorithm;
        unit = polygon;
        graphs.push_back(unit);
    }
    else if(cmd == cmdDrawPolygonIng)
    {
        int x1, y1, x2, y2;
        x1 = startPoint.x();
        y1 = startPoint.y();
        x2 = endPoint.x();
        y2 = endPoint.y();
        Line* line = new Line(x1, y1, x2, y2, color);
        int size = graphs.size();
        if(graphs[size-1]->getShape() == shapePolygon)
        {
            Polygon* polygon = (Polygon*)graphs[size-1];
            polygon->pushLine(*line);
            delete line;
        }
        else
        {
            printf("栈顶元素非多边形!\n");
        }
    }
}

void Controller::removeGraph()
{
    if(graphs.isEmpty() == false)
    {
        if(cmd == cmdDrawPolygonIng)
        {
            int size = graphs.size();
            if(graphs[size-1]->getShape() == shapePolygon)
            {
                Polygon* polygon = (Polygon*)graphs[size-1];
                polygon->popLine();
            }
            else
            {
                printf("栈顶元素非多边形!\n");
            }
        }
        else
        {
            unit = graphs[graphs.size()-1];
            delete unit;
            graphs.pop_back();
        }
    }
}

void Controller::reset()
{
    graphs.clear();
}

void Controller::paint(QPixmap* canvas)
{
    QVector<graphUnit*>::iterator i;
    for(i = graphs.begin(); i != graphs.end(); i++)
    {
        unit = *i;
        if(unit->getShape() == shapeLine)
        {
            Line* line = (Line*)unit;
            if(line != nullptr)
            {
                if(line->clipOut == false)
                {
                    if(line->lineAlgorithm == 1)
                        line->Bresenham_paint(canvas);
                    else
                        line->DDA_paint(canvas);
                }
            }
        }
        else if(unit->getShape() == shapeCircle)
        {
            Circle* circle = (Circle*)unit;
            if(circle != nullptr)
            {
                circle->paint(canvas);
            }
        }
        else if(unit->getShape() == shapeEllipse)
        {
            Ellipse* ellipse = (Ellipse*)unit;
            if(ellipse != nullptr)
            {
                ellipse->paint(canvas);
            }
        }
        else if(unit->getShape() == shapePolygon)
        {
            Polygon* polygon = (Polygon*)unit;
            if(polygon != nullptr)
            {
                polygon->paint(canvas);
            }
        }
        else if(unit->shape == shapeRectangle)
        {
            Rectangle* rectangle = (Rectangle*)unit;
            if(rectangle != nullptr)
            {
                rectangle->paint(canvas);
            }
        }
        else if(unit->shape == shapeCurve)
        {
            Curve* curve = (Curve*)unit;
            if(curve != nullptr)
            {
                if(curve->curveAlgorithm == 0)
                    curve->paint_Bezier(canvas);
                else
                    curve->paint_B_Spline(canvas);
            }
        }
    }
}

void Controller::choseGraph(graphUnit *unit, int x, int y)
{
    Shape shape = unit->shape;
    if(shape == shapeLine)
    {
        Line* line = (Line*)unit;
        if(x < (line->x1 < line->x2 ? line->x1 : line->x2))
            return;
        if(x > (line->x1 < line->x2 ? line->x2 : line->x1))
            return;
        if(y < (line->y1 < line->y2 ? line->y1 : line->y2))
            return;
        if(y > (line->y1 < line->y2 ? line->y2 : line->y1))
            return;
        if(line->x1 == line->x2)
        {
            if(abs(x - line->x1) < 5)
            {
                line->beChosen = true;
            }
            else
            {
                line->beChosen = false;
            }
            return;
        }
        else
        {   //y = (y1-y2)/(x1-x2)*(x-x1)+y1
            int y1 = (double)(line->y1-line->y2)/(double)(line->x1-line->x2)*(x-line->x1) + line->y1;
            if(abs(y-y1) < 10)
                line->beChosen = true;
            else
                line->beChosen = false;
            return;
        }
    }
    else if(shape == shapeCircle)
    {
        Circle* circle = (Circle*)unit;
        int r1 = (int)sqrt((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y));
        if(abs(r1 - circle->r) < 10)
            circle->beChosen = true;
        else
            circle->beChosen = false;
    }
    else if(shape == shapeEllipse)
    {
        Ellipse* ellipse = (Ellipse*)unit;
        int a = ellipse->rx;
        int b = ellipse->ry;
        int xe = ellipse->x;
        int ye = ellipse->y;
        double d2 = (double)((x-xe)*(x-xe))/(double)(a*a) + (double)((y-ye)*(y-ye))/(b*b);
        if( d2 < 1.05 && d2 > 0.95)
        {
            ellipse->beChosen = true;
            //printf("-test ellipse success-");
        }
        else
        {
            ellipse->beChosen = false;
            //printf("-test ellipse fail a:%d,b:%d,xe:%d,ye:%d,x:%d,y:%d,delta:%d-",
                   //a,b,xe,ye,x,y,abs(d2 - a*a*b*b));
        }
    }
    else if(shape == shapePolygon)
    {
        Polygon* polygon = (Polygon*)unit;
        int len = polygon->lineVector.size();
        for(int i = 0; i< len; i++)
        {
            Line* line = &(polygon->lineVector[i]);
            choseGraph(line,x,y);
            if(line->beChosen == true)
            {
                polygon->beChosen = true;
                line->beChosen = false;
                return;
            }
        }
        polygon->beChosen = false;
    }
    else if(shape == shapeCurve){//使用内外点测试
        Curve* curve= (Curve*)unit;
        int nCross = 0;
        int size = curve->pointVector.size();
        for(int i = 0; i < size; i++){
            int x1 = curve->pointVector[i].x();
            int y1 = curve->pointVector[i].y();
            int x2 = curve->pointVector[(i+1)%size].x();
            int y2 = curve->pointVector[(i+1)%size].y();
            if(y1 == y2)
                continue;
            if(y < ((y1 < y2)?y1:y2))
                continue;
            if(y > ((y1 > y2)?y1:y2))
                continue;
            //计算线段在p所处的y值对应的x值
            double Rm = (double)(x2 - x1) / (double)(y2-y1);
            double xLine = double(y - y1) * Rm + x1;
            if(xLine > x)
                nCross++;
        }
        if(nCross % 2 == 1)
            curve->beChosen = true;
        else
            curve->beChosen = false;
    }
}

double Controller::getAngle(int x1, int y1, int x2, int y2)
{
    if(y1 == y2 && x2 >= x1)
        return 0;
    else if(y1 == y2 && x2 < x1)
        return PI;
    else if(x1 == x2 && y2 >= y1)
        return PI*0.5;
    else if(x1 == x2 && y2 < y1)
        return PI*1.5;
    else
    {
        int deltaX = abs(x1-y1);
        int deltaY = abs(y1-y2);
        double angle = atan(deltaY/deltaX);
        if(x2 > x1 && y2 > y1)
            return angle;
        else if(x2 < x1 && y2 > y1)
            return PI - angle;
        else if(x2 < x1 && y2 < y1)
            return PI + angle;
        else
            return 2*PI - angle;
    }
}

graphUnit* Controller::getPointer(int id)
{
    int size = graphs.size();
    for(int i = 0; i < size; i++){
       if(graphs[i]->id == id)
           return graphs[i];
    }
    return nullptr;
}
