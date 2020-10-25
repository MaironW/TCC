#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGeoPath>

#include "sensor.h"
#include "linear.h"
#include "compass.h"
#include "attitudeindicator.h"
#include "portselecitionwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Linear* linearplot;

private:
    Ui::MainWindow *ui;
    PortSelecitionWindow *portDialog;
    Sensor *sensor;
    Compass *compass;
    AttitudeIndicator *attitude;

    QGeoPath path;

    int timerId;

    int verd;

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void uiUpdate();

    // Linear Plot Tab
    void on_comboBox_sensor_currentIndexChanged(int index);
    void on_checkBox_N_stateChanged(int arg1);
    void on_checkBox_E_stateChanged(int arg1);
    void on_checkBox_D_stateChanged(int arg1);
    void on_pushButton_reset_clicked();
    void on_doubleSpinBox_minscl_valueChanged(double arg1);
    void on_doubleSpinBox_maxscl_valueChanged(double arg1);
};

#endif // MAINWINDOW_H
