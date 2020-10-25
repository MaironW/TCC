#include <QTimer>

#include "linear.h"

Linear::Linear(QObject *parent) : QObject(parent)
{

}

Linear::Linear(QCustomPlot* _plotwidget){
    this->plotwidget = _plotwidget;
    this->x_data = 0;
    this->y_data = 1;
}

Linear::~Linear(){
    plotwidget->clearPlottables();
}

void Linear::realTimeDataSlot(){
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key-lastPointKey > 0.02){ // at most add point every 2 ms
      // add data to lines:
      plotwidget->graph(0)->addData(key, sensor_data->x());
      plotwidget->graph(1)->addData(key, sensor_data->y());
      plotwidget->graph(2)->addData(key, sensor_data->z());
      // rescale value (vertical) axis to fit the current data:
//      plotwidget->graph(0)->rescaleValueAxis(true);
//      plotwidget->graph(1)->rescaleValueAxis(true);
//      plotwidget->graph(2)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 10):
    plotwidget->xAxis->setRange(key, 12, Qt::AlignRight);
    plotwidget->replot();

}

void Linear::setAsTimePlot(QVector3D* _sensor){

    maxval = 10;
    minval = -10;

    plotwidget->clearPlottables();
    this->sensor_data = _sensor;

    plotwidget->addGraph(); // blue line
    plotwidget->graph(0)->setPen(QPen(QColor(0, 102, 255)));
    plotwidget->addGraph(); // red line
    plotwidget->graph(1)->setPen(QPen(QColor(204, 0, 0)));
    plotwidget->addGraph(); // green line
    plotwidget->graph(2)->setPen(QPen(QColor(51, 204, 0)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    plotwidget->xAxis->setTicker(timeTicker);
    plotwidget->axisRect()->setupFullAxesBox();
    plotwidget->yAxis->setRange(minval, maxval);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(plotwidget->xAxis, SIGNAL(rangeChanged(QCPRange)), plotwidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(plotwidget->yAxis, SIGNAL(rangeChanged(QCPRange)), plotwidget->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realTimeDataSlot()));

    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void Linear::setPlotWidget(QCustomPlot* _plotwidget){
    plotwidget = _plotwidget;
}

void Linear::setSensor(QVector3D* _sensor){
    sensor_data = _sensor;
}

void Linear::setXdata(int i){
    x_data = i;
}
void Linear::setYdata(int i){
    y_data = i;
}

void Linear::plotScale(double min, double max){
    minval = min;
    maxval = max;
    plotwidget->yAxis->setRange(minval, maxval);
}
