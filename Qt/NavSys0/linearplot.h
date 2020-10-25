#ifndef LINEARPLOT_H
#define LINEARPLOT_H

#include <QObject>
#include <QTimer>

#include "qcustomplot.h"
#include "sensor.h"

class LinearPlot : public QObject
{
public:
    explicit LinearPlot(QObject *parent = nullptr);

    LinearPlot(QCustomPlot* _plotwidget);
    ~LinearPlot();

    void setAsTimePlot(float* _sensor);     // it will plot time vs y_data

    void setPlotWidget(QCustomPlot* _plotwidget);
    void setSensor(float* _sensor);
    void setXdata(int i);
    void setYdata(int i);

    bool timeplot;

private:
    QCustomPlot* plotwidget;
    QCPCurve *curve;
    float *sensor_data;
    int x_data;
    int y_data;
    QTimer dataTimer;

signals:

public slots:

private slots:
    void realTimeDataSlot();

};

#endif // LINEARPLOT_H
