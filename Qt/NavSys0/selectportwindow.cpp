#include "selectportwindow.h"
#include "ui_selectportwindow.h"

SelectPortWindow::SelectPortWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPortWindow)
{
    ui->setupUi(this);
}

SelectPortWindow::~SelectPortWindow()
{
    delete ui;
}

void SelectPortWindow::on_pushButton_2_clicked()
{
    this->close();
    this->setResult(QDialog::Accepted);
}

void SelectPortWindow::on_pushButton_clicked()
{
    this->close();
    this->setResult(QDialog::Rejected);
}
