#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,1000);
    setWindowTitle(tr("My Board"));

    //ui->imgLabel->resize(1000,1000);

    canvasW = 600;
    canvasH = 600;
    canvas = new QPixmap(canvasW,canvasH);
    canvas->fill(Qt::white);

    unit = nullptr;
    mCmd = mouseFree;

    color = QColor(0,180,180);
    QString colorStr=QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->colorLine->setStyleSheet(colorStr);

    this->update();
}

MainWindow::~MainWindow()
{
    delete ui;
    QApplication::exit();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    delete canvas;
    canvas = new QPixmap(canvasW,canvasH);
    canvas->fill(Qt::white);
    controller.paint(canvas);
    ui->imgLabel->resize(1000,1000);//(canvasW,canvasW);
    ui->imgLabel->setPixmap(*canvas);
}

void MainWindow::on_resetButton_clicked()
{
    resetCanvasDialog* resetDialog = new resetCanvasDialog();
    resetDialog->exec();
    int w = 600, h = 600;
    if(resetDialog->isDataValid() == true)
    {
        w = resetDialog->getWidth();
        h = resetDialog->getHeight();
        resetCanvas(w,h);
        this->canvasH = h;
        this->canvasW = w;
        printf("-%d %d-",w, h);
    }
    delete resetDialog;
}

void MainWindow::on_colorButton_clicked()//从主界面获取颜色
{
    QColorDialog* colorDialog = new QColorDialog();
    color = colorDialog->getColor();
    QString colorStr=QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->colorLine->setStyleSheet(colorStr);
    delete colorDialog;
}

void MainWindow::mousePressEvent(QMouseEvent *event)//处理鼠标按下
{
    if(event->button() == Qt::LeftButton)
    {
        if(mCmd == mouseDraw){
            if(controller.cmd == cmdDrawPolygonStart){
                if(ui->polygon1->checkState() == Qt::Checked)
                    controller.polygonAlgorithm = 0;
                else if(ui->polygon2->checkState() == Qt::Checked)
                    controller.polygonAlgorithm = 1;
                else
                    controller.polygonAlgorithm = 1;

                startPoint = event->globalPos();
                startPoint = ui->imgLabel->mapFromGlobal(startPoint);
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                controller.addGraph(startPoint,endPoint,color);
                controller.cmd = cmdDrawPolygonIng;
            }
            else if(controller.cmd == cmdDrawCurveStart){
                if(ui->curve1->checkState() == Qt::Checked)
                    controller.curveAlgorithm = 0;
                else if(ui->curve2->checkState() == Qt::Checked)
                    controller.curveAlgorithm = 1;
                else
                    controller.curveAlgorithm = 1;

                startPoint = event->globalPos();
                startPoint = ui->imgLabel->mapFromGlobal(startPoint);
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                Polygon* polygon = new Polygon(color);
                controller.graphs.push_back(polygon);
                polygon->lineVector.push_back(Line(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y(),color));
                controller.cmd = cmdDrawCurveIng;
            }
            else if(controller.cmd == cmdDrawCurveIng){
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                Polygon* polygon = (Polygon*)controller.graphs[controller.graphs.size()-1];
                polygon->lineVector.push_back(Line(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y(),color));
            }
            else if(controller.cmd == cmdDrawPolygonIng)
            {
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                controller.addGraph(startPoint,endPoint,color);
            }
            else
            {
                if(controller.cmd == cmdDrawLine)//画线时，需要选择画线的算法
                {
                    if(ui->line1->checkState() == Qt::Checked)
                        controller.lineAlgorithm = 0;
                    else if(ui->line2->checkState() == Qt::Checked)
                        controller.lineAlgorithm = 1;
                    else
                        controller.lineAlgorithm = 1;
                }
                startPoint = event->globalPos();
                startPoint = ui->imgLabel->mapFromGlobal(startPoint);
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                controller.addGraph(startPoint,endPoint,color);
            }
            update();
        }
        else if(mCmd == mouseTranslate)
        {
             startPoint = event->globalPos();
             startPoint = ui->imgLabel->mapFromGlobal(startPoint);
             int x = startPoint.x();
             int y = startPoint.y();
             int len = controller.graphs.size();
             for(int i = 0; i < len; i++)
             {
                controller.choseGraph(controller.graphs[i],x,y);
                if(controller.graphs[i]->beChosen == true)
                {
                    controller.graphs[i]->lastX = x;
                    controller.graphs[i]->lastY = y;
                }
             }
        }
        else if(mCmd == mouseRotateBegin)
        {
            //Begin作用是选择旋转点
            startPoint = event->globalPos();
            startPoint = ui->imgLabel->mapFromGlobal(startPoint);
            rotatePoint = startPoint;
        }
        else if(mCmd == mouseRotateIng)
        {
            startPoint = event->globalPos();
            startPoint = ui->imgLabel->mapFromGlobal(startPoint);
            int x = startPoint.x();
            int y = startPoint.y();
            int len = controller.graphs.size();
            for(int i = 0; i < len; i++)
            {
               controller.choseGraph(controller.graphs[i],x,y);
               if(controller.graphs[i]->beChosen == true)
               {
                   Transformation::rotate(controller.graphs[i],rotatePoint.x(),rotatePoint.y(),ui->rotateBox->value()/180.0*PI);
               }
            }
        }
        else if(mCmd == mouseScaleBegin)
        {
            //Begin作用是选择缩放基准点
            startPoint = event->globalPos();
            startPoint = ui->imgLabel->mapFromGlobal(startPoint);
            scalePoint = startPoint;
        }
        else if(mCmd == mouseScaleIng)
        {
            startPoint = event->globalPos();
            startPoint = ui->imgLabel->mapFromGlobal(startPoint);
            int x = startPoint.x();
            int y = startPoint.y();
            int len = controller.graphs.size();
            for(int i = 0; i < len; i++)
            {
               controller.choseGraph(controller.graphs[i],x,y);
               if(controller.graphs[i]->beChosen == true)
               {
                   Transformation::scale(controller.graphs[i],scalePoint.x(),scalePoint.y(),ui->scaleBox->value());
               }
            }
        }
        else if(mCmd == mouseClip)
        {
            startPoint = event->globalPos();
            startPoint = ui->imgLabel->mapFromGlobal(startPoint);
            endPoint = startPoint;
            Rectangle* rectangle = new Rectangle(startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y(),
                                                 color);
            controller.graphs.push_back(rectangle);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)//处理鼠标移动
{
    if(mCmd == mouseDraw)
    {
        if(controller.cmd == cmdDrawCurveIng){
            endPoint = event->globalPos();
            endPoint = ui->imgLabel->mapFromGlobal(endPoint);
            Polygon* polygon = (Polygon*)controller.graphs[controller.graphs.size()-1];
            polygon->lineVector[polygon->lineVector.size()-1].x2 = endPoint.x();
            polygon->lineVector[polygon->lineVector.size()-1].y2 = endPoint.y();
        }
        else{
            endPoint = event->globalPos();
            endPoint = ui->imgLabel->mapFromGlobal(endPoint);
            controller.removeGraph();
            controller.addGraph(startPoint,endPoint,color);
        }
        update();
    }
    else if(mCmd == mouseTranslate)
    {
        endPoint = event->globalPos();
        endPoint = ui->imgLabel->mapFromGlobal(endPoint);
        int x = endPoint.x();
        int y = endPoint.y();
        int len = controller.graphs.size();
        for(int i = 0; i < len; i++)
        {
            if(controller.graphs[i]->beChosen == true)
            {
                int lastX = controller.graphs[i]->lastX;
                int lastY = controller.graphs[i]->lastY;
                Transformation::translate(controller.graphs[i],x-lastX,y-lastY);
                controller.graphs[i]->lastX = x;
                controller.graphs[i]->lastY = y;
            }
        }
    }
    else if(mCmd == mouseClip)
    {
        endPoint = event->globalPos();
        endPoint = ui->imgLabel->mapFromGlobal(endPoint);
        int size = controller.graphs.size();
        Rectangle* rectangle = (Rectangle*)controller.graphs[size - 1];
        rectangle->x2 = endPoint.x();
        rectangle->y2 = endPoint.y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)//处理鼠标松开
{
    if(event->button() == Qt::LeftButton)
    {
        if(mCmd == mouseDraw)
        {
            if(controller.cmd == cmdDrawPolygonIng)
            {
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                int size = controller.graphs.size();
                if(controller.graphs[size-1]->getShape() == shapePolygon)
                {
                    Polygon* polygon = (Polygon*)controller.graphs[size-1];
                    QPoint s = polygon->getStart();
                    int x1 = s.x(), y1 = s.y();
                    int x2 = endPoint.x(), y2 = endPoint.y();
                    if(abs(x1-x2)<=20 && abs(y1-y2)<=20)
                    {
                        controller.removeGraph();
                        controller.addGraph(startPoint,s,color);
                        controller.cmd = cmdDrawPolygonStart;
                    }
                    else
                    {
                        controller.removeGraph();
                        controller.addGraph(startPoint,endPoint,color);
                        controller.cmd = cmdDrawPolygonIng;
                        startPoint = endPoint;
                    }
                }
                else
                {
                    printf("栈顶元素非多边形!in function mouseReleaseEvent\n");
                }
            }
            else if(controller.cmd == cmdDrawCurveIng){
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                Polygon* polygon = (Polygon*)controller.graphs[controller.graphs.size()-1];
                int size = polygon->lineVector.size();
                polygon->lineVector[size-1].x2 = endPoint.x();
                polygon->lineVector[size-1].y2 = endPoint.y();
                //判断是否结束
                if(abs(polygon->lineVector[size-1].x1 - polygon->lineVector[size-1].x2) <= 20
                        && abs(polygon->lineVector[size-1].y1 - polygon->lineVector[size-1].y2) <= 20){
                    Curve* curve = new Curve(color);
                    for(int i = 0; i < size; i++){
                        int x = polygon->lineVector[i].x1;
                        int y = polygon->lineVector[i].y1;
                        curve->pointVector.push_back(QPoint(x,y));
                    }
                    curve->curveAlgorithm = controller.curveAlgorithm;
                    controller.graphs.pop_back();
                    controller.graphs.push_back(curve);
                    controller.cmd = cmdDrawCurveStart;
                }
                else{
                    controller.cmd = cmdDrawCurveIng;
                    startPoint = endPoint;
                }
            }
            else
            {
                endPoint = event->globalPos();
                endPoint = ui->imgLabel->mapFromGlobal(endPoint);
                controller.removeGraph();
                controller.addGraph(startPoint,endPoint,color);
            }
            update();
        }
        else if(mCmd == mouseTranslate)
        {
            endPoint = event->globalPos();
            endPoint = ui->imgLabel->mapFromGlobal(endPoint);
            int x = endPoint.x();
            int y = endPoint.y();
             int len = controller.graphs.size();
            for(int i = 0; i < len; i++)
            {
                if(controller.graphs[i]->beChosen == true)
                {
                    int lastX = controller.graphs[i]->lastX;
                    int lastY = controller.graphs[i]->lastY;
                    Transformation::translate(controller.graphs[i],x-lastX,y-lastY);
                    controller.graphs[i]->lastX = x;
                    controller.graphs[i]->lastY = y;
                    controller.graphs[i]->beChosen = false;
                }
            }
        }
        else if(mCmd == mouseRotateIng)
        {
            int len = controller.graphs.size();
            for(int i = 0; i < len; i++)
            {
                controller.graphs[i]->beChosen = false;
            }
        }
        else if(mCmd == mouseRotateBegin)
        {
            mCmd = mouseRotateIng;
        }
        else if(mCmd == mouseScaleIng)
        {
            int len = controller.graphs.size();
            for(int i = 0; i < len; i++)
            {
                controller.graphs[i]->beChosen = false;
            }
        }
        else if(mCmd == mouseScaleBegin)
        {
            mCmd = mouseScaleIng;
        }
        else if(mCmd == mouseClip)
        {
            endPoint = event->globalPos();
            endPoint = ui->imgLabel->mapFromGlobal(endPoint);
            controller.graphs.pop_back();
            if(ui->clip1->checkState() == Qt::Checked)
                controller.clipAlgorithm = 0;
            else if(ui->clip2->checkState() == Qt::Checked)
                controller.clipAlgorithm = 1;
            else
                controller.clipAlgorithm = 1;
            int size = controller.graphs.size();
            for(int i = 0; i< size; i++)
            {
                if(controller.graphs[i]->shape == shapeLine)
                {
                    if(controller.clipAlgorithm == 0)
                        Transformation::clipCohenSutherland(startPoint,endPoint, (Line*)controller.graphs[i]);
                    else
                        Transformation::clipLiangBarsky(startPoint,endPoint,(Line*)controller.graphs[i]);
                }
            }
        }
    }
}

//对应主界面鼠标操作的按钮
void MainWindow::on_cmdLineButton_clicked()
{
    mCmd = mouseDraw;
    controller.cmd = cmdDrawLine;
}
void MainWindow::on_cmdCircleButton_clicked()
{
    mCmd = mouseDraw;
    controller.cmd = cmdDrawCircle;
}
void MainWindow::on_cmdEllipseButton_clicked()
{
    mCmd = mouseDraw;
    controller.cmd = cmdDrawEllipse;
}
void MainWindow::on_cmdPolygonButton_clicked()
{
    mCmd = mouseDraw;
    controller.cmd = cmdDrawPolygonStart;
}
void MainWindow::on_cmdCurveButton_clicked()
{
    mCmd = mouseDraw;
    controller.cmd = cmdDrawCurveStart;
}

//有关图形变换的操作
void MainWindow::on_cmdTranslateButton_clicked()
{
    mCmd = mouseTranslate;
    controller.cmd = cmdFree;
}
void MainWindow::on_cmdRotateButton_clicked()
{
    mCmd = mouseRotateBegin;
    controller.cmd = cmdFree;
}
void MainWindow::on_cmdScaleButton_clicked()
{
    mCmd = mouseScaleBegin;
    controller.cmd = cmdFree;
}
void MainWindow::on_cmdClipButton_clicked()
{
    mCmd = mouseClip;
    controller.cmd = cmdFree;
}


void MainWindow::on_saveButton_clicked()//保存画布
{
    QImage img = canvas->toImage();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存画布"),
                                                    "",
                                                    tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF"));
    if(fileName.isEmpty() == false)
    {
        if(img.save(fileName) == true)
        {
            ui->statusBar->showMessage("画布保存成功");
        }
        else
        {
            QMessageBox::information(this,
                                     tr("保存画布失败"),
                                     tr("保存画布失败"));
        }
    }
}


void MainWindow::on_openButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("打开文件"),tr(""),tr("Text File (*.txt)"));
    if(fileName == "")
        return;
    fileInput(fileName,"");
}

void MainWindow::fileInput(QString fileName, QString savePath){
    this->fileName = fileName;
    this->savePath = savePath;
    if(read(fileName) == false)
        return;
    int i = 0;
    int size = list.size();
    while(i < size){
        process(i);
        this->repaint();
    }

}
bool MainWindow::read(QString name){
    list.clear();
    QFile file(name);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
        return false;
    if(file.isReadable() == false)
        return false;

    QTextStream textStream(&file);
    QString content = textStream.readAll();
    file.close();
    list = content.split('\n');
    return true;
}
void MainWindow::process(int& i){
    QString cmdStr = list[i];
    QStringList cmdList = cmdStr.split(' ');
    if(cmdList[0] == "resetCanvas"){//重置画布
        int w = cmdList[1].toInt();
        int h = cmdList[2].toInt();
        resetCanvas(w,h);
    }
    else if(cmdList[0] == "saveCanvas"){//保存画布
        saveCanvas(cmdList[1]);
    }
    else if(cmdList[0] == "setColor"){//设置画笔颜色
        int R, G, B;
        R = cmdList[1].toInt();
        G = cmdList[2].toInt();
        B = cmdList[3].toInt();
        setColor(R,G,B);
    }
    else if(cmdList[0] == "drawLine"){//绘制线段
        int id = cmdList[1].toInt();
        int x1 = cmdList[2].toInt();
        int y1 = cmdList[3].toInt();
        int x2 = cmdList[4].toInt();
        int y2 = cmdList[5].toInt();
        drawLine(id,x1,y1,x2,y2,cmdList[6]);
    }
    else if(cmdList[0] == "drawPolygon"){//绘制多边形
        int id = cmdList[1].toInt();
        int n = cmdList[2].toInt();
        QString algorithm = cmdList[3];
        i++;
        cmdList = list[i].split(' ');
        QVector<int> vector; vector.clear();
        for(int i = 0; i < 2*n; i++){
            vector.push_back(cmdList[i].toInt());
        }
        drawPolygon(id, n, algorithm, vector);
    }
    else if(cmdList[0] == "drawEllipse"){//绘制椭圆
        int id = cmdList[1].toInt();
        int x = cmdList[2].toInt();
        int y = cmdList[3].toInt();
        int rx = cmdList[4].toInt();
        int ry = cmdList[5].toInt();
        drawEllipse(id, x, y, rx, ry);
    }
    else if(cmdList[0] == "drawCurve"){//绘制曲线
        int id = cmdList[1].toInt();
        int n = cmdList[2].toInt();
        QString algorithm = cmdList[3];
        i++;
        cmdList = list[i].split(' ');
        QVector<int> vector; vector.clear();
        for(int i = 0; i < 2*n; i++){
            vector.push_back(cmdList[i].toInt());
        }
        drawCurve(id, n, algorithm, vector);
    }
    else if(cmdList[0] == "translate"){//平移
        int id =cmdList[1].toInt();
        int dx = cmdList[2].toInt();
        int dy = cmdList[3].toInt();
        translate(id, dx, dy);
    }
    else if(cmdList[0] == "rotate"){//旋转
        int id =cmdList[1].toInt();
        int x = cmdList[2].toInt();
        int y = cmdList[3].toInt();
        int r = cmdList[4].toInt();
        rotate(id, x, y, r);
    }
    else if(cmdList[0] == "scale"){//缩放
        int id =cmdList[1].toInt();
        int x = cmdList[2].toInt();
        int y = cmdList[3].toInt();
        float s = cmdList[4].toFloat();
        scale(id, x, y, s);
    }
    else if(cmdList[0] == "clip"){//裁剪
        int id = cmdList[1].toInt();
        int x1 = cmdList[2].toInt();
        int y1 = cmdList[3].toInt();
        int x2 = cmdList[4].toInt();
        int y2 = cmdList[5].toInt();
        QString algorithm = cmdList[6];
        clip(id,x1,y1,x2,y2,algorithm);
    }
    else{
        //printf("can`t analyze: %s\n",(cmdList[0].toStdString()).c_str());
    }
    i++;
}

void MainWindow::resetCanvas(int w, int h)//重置画布，供button函数和文件指令调用
{
    update();
    controller.cmd = cmdFree;
    controller.reset();
    canvasW = w;
    canvasH = h;
    update();
}
void MainWindow::saveCanvas(QString name){
    this->update();
    QString saveName = savePath + name + ".bmp";
    if(canvas->save(saveName))
        ;
    else
        printf("save failed\n");
}
void MainWindow::setColor(int R, int G, int B){
    color = QColor(R,G,B);
    QString colorStr=QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue());
    ui->colorLine->setStyleSheet(colorStr);
}
void MainWindow::drawLine(int id, int x1, int y1, int x2, int y2, QString algorithm){
    Line* line = new Line(x1, y1, x2, y2, color);
    line->id = id;
    if(algorithm == "DDA")
        line->lineAlgorithm = 0;
    else if(algorithm == "Bresenham")
        line->lineAlgorithm = 1;
    controller.graphs.push_back(line);
}
void MainWindow::drawPolygon(int id, int n, QString algorithm, QVector<int> vector){
    Polygon* polygon = new Polygon(color);
    polygon->id = id;
    if(algorithm == "DDA")
        polygon->algorithm = 0;
    else if(algorithm == "Bresenham")
        polygon->algorithm = 1;
    polygon->lineVector.clear();
    for(int i = 0; i < n; i++){
        int x1 = vector[2*i];
        int y1 = vector[2*i+1];
        int next = (i+1)%n;
        int x2 = vector[2*next];
        int y2 = vector[2*next+1];
        polygon->lineVector.push_back(Line(x1, y1, x2, y2, color));
    }
    controller.graphs.push_back(polygon);
}
void MainWindow::drawEllipse(int id, int x, int y, int rx, int ry){
    if(rx == ry){
        Circle* circle = new Circle(x, y, rx, color);
        circle->id = id;
        controller.graphs.push_back(circle);
    }
    else{
        Ellipse* ellipse = new Ellipse(x, y, rx, ry, color);
        ellipse->id = id;
        controller.graphs.push_back(ellipse);
    }
}
void MainWindow::drawCurve(int id, int n, QString algorithm, QVector<int> vector){
    Curve* curve = new Curve(color);
    curve->id = id;
    if(algorithm == "Bezier")
        curve->curveAlgorithm = 0;
    else if(algorithm == "B-spline")
        curve->curveAlgorithm = 1;
    curve->pointVector.clear();
    for(int i = 0; i < n; i++){
        int x1 = vector[2*i];
        int y1 = vector[2*i+1];
        curve->pointVector.push_back(QPoint(x1,y1));
    }
    controller.graphs.push_back(curve);
}
void MainWindow::translate(int id, int dx, int dy){
    graphUnit* p = controller.getPointer(id);
    if(p == nullptr){
        printf("id(%d) doesn`t exists\n",id);
        return;
    }
    Transformation::translate(p, dx, dy);
}
void MainWindow::rotate(int id, int x, int y, int r){
    graphUnit* p = controller.getPointer(id);
    if(p == nullptr){
        printf("id(%d) doesn`t exists\n",id);
        return;
    }
    Transformation::rotate(p, x, y, (double)r/180.0*PI);
}
void MainWindow::scale(int id, int x, int y, float s){
    graphUnit* p = controller.getPointer(id);
    if(p == nullptr){
        printf("id(%d) doesn`t exists\n",id);
        return;
    }
    Transformation::scale(p,x,y,s);
}
void MainWindow::clip(int id, int x1, int y1, int x2, int y2, QString algorithm){
    graphUnit* p = controller.getPointer(id);
    if(p == nullptr){
        printf("id(%d) doesn`t exists\n",id);
        return;
    }
    QPoint point1 = QPoint(x1, y1);
    QPoint point2 = QPoint(x2, y2);
    if(algorithm == "Cohen-Sutherland")
        Transformation::clipCohenSutherland(point1, point2, (Line*)p);
    else if(algorithm == "Liang-Barsky")
        Transformation::clipLiangBarsky(point1, point2, (Line*)p);
}
