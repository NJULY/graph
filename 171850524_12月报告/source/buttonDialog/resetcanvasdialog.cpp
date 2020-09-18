#include "resetCanvasDialog.h"
#include "ui_resetcanvasdialog.h"

resetCanvasDialog::resetCanvasDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resetCanvasDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("获取画布尺寸"));
    valid = false;
    width = 100;
    height = 100;
}

resetCanvasDialog::~resetCanvasDialog()
{
    delete ui;
}

void resetCanvasDialog::on_okButton_clicked()
{
    width = ui->widthSpinBox->value();
    height = ui->heightSpinBox->value();
    //printf("--%d %d--", width, height);
    valid = true;
    close();
}

void resetCanvasDialog::on_cancleButton_clicked()
{
    valid = false;
    close();
}

bool resetCanvasDialog::isDataValid()
{
    return valid;
}

int resetCanvasDialog::getWidth()
{
    return width;
}

int resetCanvasDialog::getHeight()
{
    return height;
}
