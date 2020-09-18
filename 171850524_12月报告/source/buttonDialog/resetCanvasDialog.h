#ifndef RESETCANVASDIALOG_H
#define RESETCANVASDIALOG_H

#include <QDialog>

namespace Ui {
class resetCanvasDialog;
}

class resetCanvasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit resetCanvasDialog(QWidget *parent = nullptr);
    ~resetCanvasDialog();
    bool isDataValid();
    int getWidth();
    int getHeight();
private slots:
    void on_okButton_clicked();

    void on_cancleButton_clicked();

private:
    Ui::resetCanvasDialog *ui;
    bool valid;
    int width;
    int height;
};

#endif // RESETCANVASDIALOG_H
