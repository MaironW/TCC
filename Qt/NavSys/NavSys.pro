#-------------------------------------------------
#
# Project created by QtCreator 2020-03-16T13:41:47
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += bluetooth
QT       += location
QT       += positioning
QT       += quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = NavSys
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sensor.cpp \
    qcustomplot.cpp \
    linear.cpp \
    compass.cpp \
    attitudeindicator.cpp \
    portselecitionwindow.cpp

HEADERS += \
        mainwindow.h \
    sensor.h \
    qcustomplot.h \
    linear.h \
    compass.h \
    attitudeindicator.h \
    portselecitionwindow.h

FORMS += \
        mainwindow.ui \
    portselecitionwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc


QML_IMPORT_PATH += $$PWD
