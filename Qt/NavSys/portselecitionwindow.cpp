#include "portselecitionwindow.h"
#include "ui_portselecitionwindow.h"

PortSelecitionWindow::PortSelecitionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortSelecitionWindow)
{
    ui->setupUi(this);
    USB = true;
}

PortSelecitionWindow::~PortSelecitionWindow()
{
    delete ui;
}

void PortSelecitionWindow::on_radioButton_clicked()
{
    USB = true;
}

void PortSelecitionWindow::on_radioButton_2_clicked()
{
    USB = false;
}

void PortSelecitionWindow::on_buttonBox_accepted()
{

}
