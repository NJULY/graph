#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if(argc == 3){
        w.fileInput(argv[1],argv[2]);
        w.close();
        a.exit(0);
    }
    else{
        return a.exec();
    }
}
