#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QDialog>
#include <QVector>
#include <QString>
#include <QStack>
#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include "graphUnit/graphUnit.h"
#include "graphUnit/line.h"
#include "graphUnit/circle.h"
#include "graphUnit/ellipse.h"
#include "graphUnit/rectangle.h"
#include "graphUnit/curve.h"
#include "buttonDialog/resetCanvasDialog.h"
#include "controller.h"
#include "transformation.h"

enum mouseCommand {mouseFree,mouseDraw,
                   mouseTranslate, mouseRotateBegin, mouseRotateIng,
                   mouseScaleBegin, mouseScaleIng,
                   mouseClip
                  };
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fileInput(QString fileName, QString savePath);
protected:
    void paintEvent(QPaintEvent *event);//重写绘图函数
    void mousePressEvent(QMouseEvent *event);//处理鼠标按下
    void mouseMoveEvent(QMouseEvent *event);//处理鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//处理鼠标松开
private slots:
    void on_openButton_clicked();//打开画布
    void on_saveButton_clicked();//保存画布
    void on_resetButton_clicked();//重置画布
    void on_colorButton_clicked();//选择颜色
    void on_cmdLineButton_clicked();//鼠标画线
    void on_cmdCircleButton_clicked();//鼠标画圆
    void on_cmdEllipseButton_clicked();//鼠标画椭圆
    void on_cmdPolygonButton_clicked();//鼠标画多边形
    void on_cmdTranslateButton_clicked();
    void on_cmdRotateButton_clicked();
    void on_cmdScaleButton_clicked();
    void on_cmdClipButton_clicked();
    void on_cmdCurveButton_clicked();

private:
    Ui::MainWindow *ui;
    //自定义所需变量
    QPixmap* canvas;//画布
    graphUnit* unit;
    QColor color;//主界面选择的颜色
    mouseCommand mCmd;//鼠标命令
    Controller controller;//图元控制
    QPoint startPoint;
    QPoint endPoint;
    QPoint rotatePoint;//旋转基准点
    QPoint scalePoint;//缩放基准点

    int canvasW;
    int canvasH;

    QString fileName;
    QString savePath;
    QStringList list;
    bool read(QString fileName);
    void process(int& i);
    void resetCanvas(int w, int h);
    void saveCanvas(QString name);
    void setColor(int R, int G, int B);
    void drawLine(int id, int x1, int y1, int x2, int y2, QString algorithm);
    void drawPolygon(int id, int n, QString algorithm, QVector<int> vector);
    void drawEllipse(int id, int x, int y, int rx, int ry);
    void drawCurve(int id, int n, QString algorithm, QVector<int> vector);
    void translate(int id, int dx, int dy);
    void rotate(int id, int x, int y, int r);
    void scale(int id, int x, int y, float s);
    void clip(int id, int x1, int y1, int x2, int y2, QString algorithm);
};

#endif // MAINWINDOW_H
