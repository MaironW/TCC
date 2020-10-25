#include <QTimer>

#include "linearplot.h"

LinearPlot::LinearPlot(QObject *parent) : QObject(parent)
{

}

LinearPlot::LinearPlot(QCustomPlot* _plotwidget){
    this->plotwidget = _plotwidget;
    this->x_data = 0;
    this->y_data = 1;
}

LinearPlot::~LinearPlot(){
    plotwidget->clearPlottables();
}

void LinearPlot::realTimeDataSlot(){
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002){ // at most add point every 2 ms
      // add data to lines:
      plotwidget->graph(0)->addData(key, sensor_data[0]);
      plotwidget->graph(1)->addData(key, sensor_data[1]);
      plotwidget->graph(2)->addData(key, sensor_data[2]);
      // rescale value (vertical) axis to fit the current data:
      plotwidget->graph(0)->rescaleValueAxis(true);
      plotwidget->graph(1)->rescaleValueAxis(true);
      plotwidget->graph(2)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    plotwidget->xAxis->setRange(key, 10, Qt::AlignRight);
    plotwidget->replot();

}

void LinearPlot::setAsTimePlot(float* _sensor){

    plotwidget->clearPlottables();
    this->sensor_data = _sensor;

    plotwidget->addGraph(); // blue line
    plotwidget->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    plotwidget->addGraph(); // red line
    plotwidget->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    plotwidget->addGraph(); // green line
    plotwidget->graph(2)->setPen(QPen(QColor(40, 255, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    plotwidget->xAxis->setTicker(timeTicker);
    plotwidget->axisRect()->setupFullAxesBox();
    plotwidget->yAxis->setRange(-1.5, 1.5);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(plotwidget->xAxis, SIGNAL(rangeChanged(QCPRange)), plotwidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(plotwidget->yAxis, SIGNAL(rangeChanged(QCPRange)), plotwidget->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realTimeDataSlot()));

    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void LinearPlot::setPlotWidget(QCustomPlot* _plotwidget){
    plotwidget = _plotwidget;
}

void LinearPlot::setSensor(float* _sensor){
    sensor_data = _sensor;
}

void LinearPlot::setXdata(int i){
    x_data = i;
}
void LinearPlot::setYdata(int i){
    y_data = i;
}
