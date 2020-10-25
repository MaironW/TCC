#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sensor.h"
#include "linear.h"
#include "compass.h"
#include "attitudeindicator.h"
#include "selectportwindow.h"

#include <QDebug>
#include <QQuickWidget>
#include <QQmlContext>
#include <QGeoPath>
#include <cmath>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    portDialog = new SelectPortWindow(this);
    portDialog->setModal(true);
    portDialog->exec();

    timerId = startTimer(100);

    ui->setupUi(this);

    sensor = new Sensor(portDialog->result());
    connect(sensor, &Sensor::dataUpdated, this, &MainWindow::uiUpdate);

    linearplot = new Linear(ui->widget_linearplot);
    linearplot->setAsTimePlot(&sensor->d);

    ui->widget_attitude->setAttitude(&sensor->euler);

//    ui->widget_map->rootContext()->setContextProperty("routeline",QVariant::fromValue(sensor->coord));
    ui->widget_map->rootContext()->setContextProperty("lastposition",QVariant::fromValue(path.coordinateAt(path.size()-1)));
    ui->widget_map->rootContext()->setContextProperty("heading",QVariant::fromValue(sensor->euler.z()));
    ui->widget_map->setSource(QUrl(QStringLiteral("qrc:/map.qml")));

    verd = 0;
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}

void MainWindow::uiUpdate(){
    if(ui->tab_linear->isVisible()){
        ui->label_nvalue->setText(QString("%1").arg(linearplot->sensor_data->x(),0,'F',2)+sensor->unit.at(ui->comboBox_sensor->currentIndex()));
        ui->label_evalue->setText(QString("%1").arg(linearplot->sensor_data->y(),0,'F',2)+sensor->unit.at(ui->comboBox_sensor->currentIndex()));
        ui->label_dvalue->setText(QString("%1").arg(linearplot->sensor_data->z(),0,'F',2)+sensor->unit.at(ui->comboBox_sensor->currentIndex()));
    }else if(ui->tab_cockpit->isVisible()){
        ui->widget_compass->setHeading(-sensor->euler.z());
        ui->widget_compass->update();
        ui->label_headingvalue->setText(QString("%1°").arg(sensor->euler.z(),0,'F',2));
        ui->label_headingvalue_2->setText(QString("%1°").arg(std::fmod(sensor->euler.z()+360,360),0,'F',2));

        ui->widget_attitude->update();
        ui->label_bankvalue->setText(QString("%1°").arg(sensor->euler.x(),0,'F',2));
        ui->label_pitchvalue->setText(QString("%1°").arg(sensor->euler.y(),0,'F',2));

        ui->label_lat_value->setText(QString("%1°").arg(sensor->coord.coordinateAt(sensor->coord.size()-1).latitude(),0,'F',6));
        ui->label_long_value->setText(QString("%1°").arg(sensor->coord.coordinateAt(sensor->coord.size()-1).longitude(),0,'F',6));
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
//    ui->widget_map->rootContext()->setContextProperty("routeline",QVariant::fromValue(sensor->coord));
    ui->widget_map->rootContext()->setContextProperty("heading",QVariant::fromValue(sensor->euler.z()));
    ui->widget_map->rootContext()->setContextProperty("lastposition",QVariant::fromValue(sensor->coord.coordinateAt(sensor->coord.size()-1)));
//    ui->widget_map->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
}



void MainWindow::on_comboBox_sensor_currentIndexChanged(int index)
{
    delete linearplot;
    linearplot = new Linear(ui->widget_linearplot);
    linearplot->setAsTimePlot(sensor->sensor_list.at(index));

    if(index==3){
        ui->label_N->setText("Roll:");
        ui->label_E->setText("Pitch:");
        ui->label_D->setText("Yaw:");
    }else{
        ui->label_N->setText("N:");
        ui->label_E->setText("E:");
        ui->label_D->setText("D:");
    }
}

void MainWindow::on_checkBox_N_stateChanged(int arg1)
{
    ui->widget_linearplot->graph(0)->setVisible(arg1);
}

void MainWindow::on_checkBox_E_stateChanged(int arg1)
{
    ui->widget_linearplot->graph(1)->setVisible(arg1);
}

void MainWindow::on_checkBox_D_stateChanged(int arg1)
{
    ui->widget_linearplot->graph(2)->setVisible(arg1);
}

void MainWindow::on_pushButton_reset_clicked()
{
    linearplot->plotScale(-10,10);
    ui->doubleSpinBox_minscl->setValue(linearplot->minval);
    ui->doubleSpinBox_maxscl->setValue(linearplot->maxval);
}

void MainWindow::on_doubleSpinBox_minscl_valueChanged(double arg1)
{
    linearplot->plotScale(arg1,linearplot->maxval);
}

void MainWindow::on_doubleSpinBox_maxscl_valueChanged(double arg1)
{
    linearplot->plotScale(linearplot->minval,arg1);
}
