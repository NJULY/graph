#ifndef GRAPHUNITCONTROLLER_H
#define GRAPHUNITCONTROLLER_H
#include "graphUnit.h"
#include "mainwindow.h"
#include <QVector>


class GraphUnitController
{
public:
    GraphUnitController();
    void addGraph(QPoint startPoint, QPoint endPoint, QColor color, Command cmd);
    void removeGraph();
private:
    QVector<graphUnit*> graphs;//存储所有的图元
    graphUnit* unit;
};

#endif // GRAPHUNITCONTROLLER_H
