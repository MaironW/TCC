#include "sensor.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QtBluetooth>

#include <QDebug>



Sensor::Sensor(QObject *parent) : QObject(parent)
{
//    serialSetup();
}

Sensor::Sensor(bool port)
{
    serialSetup(port);
}


Sensor::~Sensor(){
    serial->close();
}

void Sensor::serialSetup(bool port){

    if(port){
        qDebug() << "USB port selected.";
        serial = new QSerialPort("/dev/ttyUSB0");   // use this for USB
    }else{
        qDebug() << "Bluetooth port selected.";
        serial = new QSerialPort("/dev/tnt0");      // use this for bluetooth
    }

    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);

    if(serial->open(QIODevice::ReadOnly)){
        qDebug() << serial->portName();
        qDebug() << serial->openMode();
    }else{
        qDebug() << serial->errorString();
        qDebug() << serial->error();
    }

    sensor_list.append(&d);
    sensor_list.append(&v);
    sensor_list.append(&a);
    sensor_list.append(&euler);

    unit.append("m");
    unit.append("m/s");
    unit.append("m/s²");
    unit.append("°");

    connect(serial, &QSerialPort::readyRead, this, &Sensor::readDataSerial);

    if(!port){
        serial->setBaudRate(QSerialPort::Baud115200);
    }
}

void Sensor::readDataSerial(){
    char data[150];
//    char data[62];
    QList<QString> data_arr;

    if(serial->canReadLine()){
        serial->readLine(data,150);
//        serial->readLine(data,62);
        data_arr = QString(data).split(',',QString::SkipEmptyParts);

        if(data_arr.size()==15){
            d.setX(data_arr[0].toDouble());
            d.setY(data_arr[1].toDouble());
            d.setZ(data_arr[2].toDouble());
            v.setX(data_arr[3].toDouble());
            v.setY(data_arr[4].toDouble());
            v.setZ(data_arr[5].toDouble());
            a.setX(data_arr[6].toDouble());
            a.setY(data_arr[7].toDouble());
            a.setZ(data_arr[8].toDouble());

            qn.setScalar(data_arr[9].toDouble());
            qn.setVector(data_arr[10].toDouble(),
                        data_arr[11].toDouble(),
                        data_arr[12].toDouble());

            float heading = -qn.toEulerAngles().z();

            // get attitde in body frame
            qb = qn.conjugated() * QQuaternion::fromAxisAndAngle(QVector3D(0,0,1),-heading);

            euler = qb.toEulerAngles(); // euler angles in degrees

            euler.setX(euler.x());
            euler.setY(euler.y());
            euler.setZ(heading);

            // check valid GPS data ALTERAR ESSES LIMITES AQUI DEPOIS
            if(abs(data_arr[13].toDouble())<30 && abs(data_arr[14].toDouble())<60){
                    coord.addCoordinate(QGeoCoordinate(data_arr[13].toDouble(),data_arr[14].toDouble()));
            }



            emit dataUpdated();
        //}else
//        if(data_arr.size()==6){
//            data_arr = QString(data).split(',',QString::SkipEmptyParts);

//            // check valid GPS data ALTERAR ESSES LIMITES AQUI DEPOIS
//            if(abs(data_arr[0].toDouble())<30 && abs(data_arr[1].toDouble())<60){
//                    coord.addCoordinate(QGeoCoordinate(data_arr[0].toDouble(),data_arr[1].toDouble()));
//            }

//            qn.setScalar(data_arr[2].toDouble());
//            qn.setVector(data_arr[3].toDouble(),
//                        data_arr[4].toDouble(),
//                        data_arr[5].toDouble());

//            float heading = -qn.toEulerAngles().z();

//            // get attitde in body frame
//            qb = qn.conjugated() * QQuaternion::fromAxisAndAngle(QVector3D(0,0,1),-heading);

//            euler = qb.toEulerAngles(); // euler angles in degrees

//            euler.setX(euler.x());
//            euler.setY(euler.y());
//            euler.setZ(heading);

////            qDebug() << data_arr;
////            qDebug() << qb;

//            emit dataUpdated();
        }else if(data_arr.size()==1 && data_arr[0]!="\n"){
            qDebug() << data_arr;
        }
    }
}
