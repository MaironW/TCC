#ifndef Linear_H
#define Linear_H

#include <QObject>
#include <QTimer>

#include "qcustomplot.h"
#include "sensor.h"

class Linear : public QObject
{
    Q_OBJECT
public:
    explicit Linear(QObject *parent = nullptr);

    Linear(QCustomPlot* _plotwidget);
    ~Linear();

    void setAsTimePlot(QVector3D* _sensor);     // it will plot time vs y_data

    void setPlotWidget(QCustomPlot* _plotwidget);
    void setSensor(QVector3D* _sensor);
    void setXdata(int i);
    void setYdata(int i);

    void plotScale(double min, double max);

    bool timeplot;

    QVector3D* sensor_data;

    double maxval;
    double minval;

private:
    QCustomPlot* plotwidget;
    QCPCurve *curve;
    int x_data;
    int y_data;
    QTimer dataTimer;



signals:

public slots:

private slots:
    void realTimeDataSlot();

};

#endif // Linear_H
