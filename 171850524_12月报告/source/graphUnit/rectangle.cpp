#include "rectangle.h"

Rectangle::Rectangle()
{
    shape = shapeRectangle;
    color = QColor(0,200,200);
}

Rectangle::Rectangle(int x1, int y1, int x2, int y2, QColor color)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->color = color;
    this->shape = shapeRectangle;
}

void Rectangle::paint(QPixmap *canvas)
{
    QPainter* canvasPainter=new QPainter(canvas);
    QPen pen = canvasPainter->pen();
    pen.setColor(color);
    canvasPainter->setPen(pen);

    int xmin = x1 < x2 ? x1 : x2;
    int xmax = x1 < x2 ? x2 : x1;
    int ymin = y1 < y2 ? y1 : y2;
    int ymax = y1 < y2 ? y2 : y1;
    for(int i = xmin; i <= xmax; i++)
    {
        canvasPainter->drawPoint(QPoint(i, ymin));
        canvasPainter->drawPoint(QPoint(i, ymax));
    }
    for(int i = ymin; i<= ymax; i++)
    {
        canvasPainter->drawPoint(QPoint(xmin, i));
        canvasPainter->drawPoint(QPoint(xmax, i));
    }

    delete canvasPainter;
}
