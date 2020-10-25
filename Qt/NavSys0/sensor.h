#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QQuaternion>

#include <QtBluetooth>
#include <QGeoPath>

class Sensor : public QObject
{
    Q_OBJECT
public:
    explicit Sensor(bool port);
    ~Sensor();

    QList<QVector3D*> sensor_list;
    QVector3D d; // displacement data
    QVector3D v; // velocity data
    QVector3D a; // acceleration data
    QGeoPath coord; // coordinate data

    QQuaternion qn;  // navigation attitude in quaternions format
    QQuaternion qb;  // body attitude in quaternions format

    QVector3D euler;  // roll, pitch, yaw

    QList<QString> unit;


private:
    QSerialPort *serial;
    void serialSetup(bool port);

signals:
    void dataUpdated();

public slots:
    void readDataSerial();
};

#endif // SENSOR_H
