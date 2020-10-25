/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "QtQuickWidgets/QQuickWidget"
#include "attitudeindicator.h"
#include "compass.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionTeste;
    QAction *actionBeleza;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_linear;
    QCustomPlot *widget_linearplot;
    QGroupBox *groupBox_plotsettings;
    QLabel *label_sensor;
    QComboBox *comboBox_sensor;
    QPushButton *pushButton_reset;
    QLabel *label_axis;
    QCheckBox *checkBox_N;
    QCheckBox *checkBox_E;
    QCheckBox *checkBox_D;
    QDoubleSpinBox *doubleSpinBox_maxscl;
    QDoubleSpinBox *doubleSpinBox_minscl;
    QLabel *label_axis_min;
    QLabel *label_axis_max;
    QGroupBox *groupBox_values;
    QLabel *label_N;
    QLabel *label_E;
    QLabel *label_D;
    QLabel *label_nvalue;
    QLabel *label_evalue;
    QLabel *label_dvalue;
    QWidget *tab_cockpit;
    QGroupBox *groupBox_compass;
    Compass *widget_compass;
    QLabel *label_heading;
    QLabel *label_headingvalue;
    QGroupBox *groupBox_ah;
    AttitudeIndicator *widget_attitude;
    QLabel *label_bank;
    QLabel *label_bankvalue;
    QLabel *label_pitch;
    QLabel *label_pitchvalue;
    QGroupBox *groupBox;
    QQuickWidget *widget_map;
    QLabel *label_lat;
    QLabel *label_long;
    QLabel *label_lat_value;
    QLabel *label_long_value;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1025, 460);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1025, 460));
        MainWindow->setMaximumSize(QSize(1025, 460));
        actionTeste = new QAction(MainWindow);
        actionTeste->setObjectName(QStringLiteral("actionTeste"));
        actionBeleza = new QAction(MainWindow);
        actionBeleza->setObjectName(QStringLiteral("actionBeleza"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 1024, 411));
        tab_linear = new QWidget();
        tab_linear->setObjectName(QStringLiteral("tab_linear"));
        widget_linearplot = new QCustomPlot(tab_linear);
        widget_linearplot->setObjectName(QStringLiteral("widget_linearplot"));
        widget_linearplot->setGeometry(QRect(20, 10, 701, 351));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_linearplot->sizePolicy().hasHeightForWidth());
        widget_linearplot->setSizePolicy(sizePolicy1);
        groupBox_plotsettings = new QGroupBox(tab_linear);
        groupBox_plotsettings->setObjectName(QStringLiteral("groupBox_plotsettings"));
        groupBox_plotsettings->setGeometry(QRect(740, 150, 171, 221));
        label_sensor = new QLabel(groupBox_plotsettings);
        label_sensor->setObjectName(QStringLiteral("label_sensor"));
        label_sensor->setGeometry(QRect(10, 30, 67, 17));
        comboBox_sensor = new QComboBox(groupBox_plotsettings);
        comboBox_sensor->addItem(QString());
        comboBox_sensor->addItem(QString());
        comboBox_sensor->addItem(QString());
        comboBox_sensor->addItem(QString());
        comboBox_sensor->setObjectName(QStringLiteral("comboBox_sensor"));
        comboBox_sensor->setGeometry(QRect(10, 50, 151, 25));
        pushButton_reset = new QPushButton(groupBox_plotsettings);
        pushButton_reset->setObjectName(QStringLiteral("pushButton_reset"));
        pushButton_reset->setGeometry(QRect(10, 140, 151, 25));
        label_axis = new QLabel(groupBox_plotsettings);
        label_axis->setObjectName(QStringLiteral("label_axis"));
        label_axis->setGeometry(QRect(10, 90, 67, 17));
        checkBox_N = new QCheckBox(groupBox_plotsettings);
        checkBox_N->setObjectName(QStringLiteral("checkBox_N"));
        checkBox_N->setGeometry(QRect(20, 110, 61, 23));
        checkBox_N->setChecked(true);
        checkBox_E = new QCheckBox(groupBox_plotsettings);
        checkBox_E->setObjectName(QStringLiteral("checkBox_E"));
        checkBox_E->setGeometry(QRect(60, 110, 61, 23));
        checkBox_E->setChecked(true);
        checkBox_D = new QCheckBox(groupBox_plotsettings);
        checkBox_D->setObjectName(QStringLiteral("checkBox_D"));
        checkBox_D->setGeometry(QRect(100, 110, 61, 23));
        checkBox_D->setChecked(true);
        doubleSpinBox_maxscl = new QDoubleSpinBox(groupBox_plotsettings);
        doubleSpinBox_maxscl->setObjectName(QStringLiteral("doubleSpinBox_maxscl"));
        doubleSpinBox_maxscl->setGeometry(QRect(90, 190, 69, 26));
        doubleSpinBox_maxscl->setMinimum(-2000);
        doubleSpinBox_maxscl->setMaximum(2000);
        doubleSpinBox_maxscl->setSingleStep(0.5);
        doubleSpinBox_maxscl->setValue(10);
        doubleSpinBox_minscl = new QDoubleSpinBox(groupBox_plotsettings);
        doubleSpinBox_minscl->setObjectName(QStringLiteral("doubleSpinBox_minscl"));
        doubleSpinBox_minscl->setGeometry(QRect(10, 190, 69, 26));
        doubleSpinBox_minscl->setMinimum(-2000);
        doubleSpinBox_minscl->setMaximum(2000);
        doubleSpinBox_minscl->setSingleStep(0.5);
        doubleSpinBox_minscl->setValue(-10);
        label_axis_min = new QLabel(groupBox_plotsettings);
        label_axis_min->setObjectName(QStringLiteral("label_axis_min"));
        label_axis_min->setGeometry(QRect(10, 170, 67, 17));
        label_axis_max = new QLabel(groupBox_plotsettings);
        label_axis_max->setObjectName(QStringLiteral("label_axis_max"));
        label_axis_max->setGeometry(QRect(80, 170, 67, 17));
        groupBox_values = new QGroupBox(tab_linear);
        groupBox_values->setObjectName(QStringLiteral("groupBox_values"));
        groupBox_values->setGeometry(QRect(740, 20, 171, 121));
        label_N = new QLabel(groupBox_values);
        label_N->setObjectName(QStringLiteral("label_N"));
        label_N->setGeometry(QRect(10, 30, 67, 17));
        label_E = new QLabel(groupBox_values);
        label_E->setObjectName(QStringLiteral("label_E"));
        label_E->setGeometry(QRect(10, 60, 67, 17));
        label_D = new QLabel(groupBox_values);
        label_D->setObjectName(QStringLiteral("label_D"));
        label_D->setGeometry(QRect(10, 90, 67, 17));
        label_nvalue = new QLabel(groupBox_values);
        label_nvalue->setObjectName(QStringLiteral("label_nvalue"));
        label_nvalue->setGeometry(QRect(36, 30, 111, 16));
        label_nvalue->setLayoutDirection(Qt::LeftToRight);
        label_nvalue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_nvalue->setWordWrap(false);
        label_evalue = new QLabel(groupBox_values);
        label_evalue->setObjectName(QStringLiteral("label_evalue"));
        label_evalue->setGeometry(QRect(36, 60, 111, 16));
        label_evalue->setLayoutDirection(Qt::LeftToRight);
        label_evalue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_dvalue = new QLabel(groupBox_values);
        label_dvalue->setObjectName(QStringLiteral("label_dvalue"));
        label_dvalue->setGeometry(QRect(36, 90, 111, 16));
        label_dvalue->setLayoutDirection(Qt::LeftToRight);
        label_dvalue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        tabWidget->addTab(tab_linear, QString());
        tab_cockpit = new QWidget();
        tab_cockpit->setObjectName(QStringLiteral("tab_cockpit"));
        groupBox_compass = new QGroupBox(tab_cockpit);
        groupBox_compass->setObjectName(QStringLiteral("groupBox_compass"));
        groupBox_compass->setGeometry(QRect(20, 10, 321, 361));
        widget_compass = new Compass(groupBox_compass);
        widget_compass->setObjectName(QStringLiteral("widget_compass"));
        widget_compass->setGeometry(QRect(10, 50, 301, 300));
        widget_compass->setAutoFillBackground(false);
        label_heading = new QLabel(groupBox_compass);
        label_heading->setObjectName(QStringLiteral("label_heading"));
        label_heading->setGeometry(QRect(10, 30, 67, 17));
        label_headingvalue = new QLabel(groupBox_compass);
        label_headingvalue->setObjectName(QStringLiteral("label_headingvalue"));
        label_headingvalue->setGeometry(QRect(80, 30, 61, 17));
        label_headingvalue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_ah = new QGroupBox(tab_cockpit);
        groupBox_ah->setObjectName(QStringLiteral("groupBox_ah"));
        groupBox_ah->setGeometry(QRect(350, 10, 321, 361));
        widget_attitude = new AttitudeIndicator(groupBox_ah);
        widget_attitude->setObjectName(QStringLiteral("widget_attitude"));
        widget_attitude->setGeometry(QRect(10, 50, 300, 300));
        label_bank = new QLabel(groupBox_ah);
        label_bank->setObjectName(QStringLiteral("label_bank"));
        label_bank->setGeometry(QRect(10, 30, 67, 17));
        label_bankvalue = new QLabel(groupBox_ah);
        label_bankvalue->setObjectName(QStringLiteral("label_bankvalue"));
        label_bankvalue->setGeometry(QRect(50, 30, 61, 17));
        label_bankvalue->setScaledContents(false);
        label_bankvalue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_pitch = new QLabel(groupBox_ah);
        label_pitch->setObjectName(QStringLiteral("label_pitch"));
        label_pitch->setGeometry(QRect(160, 30, 67, 17));
        label_pitchvalue = new QLabel(groupBox_ah);
        label_pitchvalue->setObjectName(QStringLiteral("label_pitchvalue"));
        label_pitchvalue->setGeometry(QRect(200, 30, 61, 17));
        label_pitchvalue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox = new QGroupBox(tab_cockpit);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(680, 10, 321, 361));
        widget_map = new QQuickWidget(groupBox);
        widget_map->setObjectName(QStringLiteral("widget_map"));
        widget_map->setGeometry(QRect(10, 50, 300, 300));
        widget_map->setResizeMode(QQuickWidget::SizeRootObjectToView);
        label_lat = new QLabel(groupBox);
        label_lat->setObjectName(QStringLiteral("label_lat"));
        label_lat->setGeometry(QRect(10, 30, 67, 17));
        label_long = new QLabel(groupBox);
        label_long->setObjectName(QStringLiteral("label_long"));
        label_long->setGeometry(QRect(170, 30, 81, 17));
        label_lat_value = new QLabel(groupBox);
        label_lat_value->setObjectName(QStringLiteral("label_lat_value"));
        label_lat_value->setGeometry(QRect(70, 30, 67, 17));
        label_lat_value->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_long_value = new QLabel(groupBox);
        label_long_value->setObjectName(QStringLiteral("label_long_value"));
        label_long_value->setGeometry(QRect(240, 30, 67, 17));
        label_long_value->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        tabWidget->addTab(tab_cockpit, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1025, 22));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Mairon UAV", nullptr));
        actionTeste->setText(QApplication::translate("MainWindow", "Teste", nullptr));
        actionBeleza->setText(QApplication::translate("MainWindow", "Beleza", nullptr));
        groupBox_plotsettings->setTitle(QApplication::translate("MainWindow", "Plot Settings", nullptr));
        label_sensor->setText(QApplication::translate("MainWindow", "Sensor:", nullptr));
        comboBox_sensor->setItemText(0, QApplication::translate("MainWindow", "Displacement", nullptr));
        comboBox_sensor->setItemText(1, QApplication::translate("MainWindow", "Velocity", nullptr));
        comboBox_sensor->setItemText(2, QApplication::translate("MainWindow", "Acceleration", nullptr));
        comboBox_sensor->setItemText(3, QApplication::translate("MainWindow", "Euler Angles", nullptr));

        pushButton_reset->setText(QApplication::translate("MainWindow", "Rescale", nullptr));
        label_axis->setText(QApplication::translate("MainWindow", "Axis:", nullptr));
        checkBox_N->setText(QApplication::translate("MainWindow", "N", nullptr));
        checkBox_E->setText(QApplication::translate("MainWindow", "E", nullptr));
        checkBox_D->setText(QApplication::translate("MainWindow", "D", nullptr));
        label_axis_min->setText(QApplication::translate("MainWindow", "Min:", nullptr));
        label_axis_max->setText(QApplication::translate("MainWindow", "Max:", nullptr));
        groupBox_values->setTitle(QApplication::translate("MainWindow", "Values", nullptr));
        label_N->setText(QApplication::translate("MainWindow", "N:", nullptr));
        label_E->setText(QApplication::translate("MainWindow", "E:", nullptr));
        label_D->setText(QApplication::translate("MainWindow", "D:", nullptr));
        label_nvalue->setText(QApplication::translate("MainWindow", "n_value", nullptr));
        label_evalue->setText(QApplication::translate("MainWindow", "e_value", nullptr));
        label_dvalue->setText(QApplication::translate("MainWindow", "d_value", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_linear), QApplication::translate("MainWindow", "Linear", nullptr));
        groupBox_compass->setTitle(QApplication::translate("MainWindow", "Compass", nullptr));
        label_heading->setText(QApplication::translate("MainWindow", "Heading:", nullptr));
        label_headingvalue->setText(QApplication::translate("MainWindow", "0", nullptr));
        groupBox_ah->setTitle(QApplication::translate("MainWindow", "Attitude Indicator", nullptr));
        label_bank->setText(QApplication::translate("MainWindow", "Bank:", nullptr));
        label_bankvalue->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_pitch->setText(QApplication::translate("MainWindow", "Pitch:", nullptr));
        label_pitchvalue->setText(QApplication::translate("MainWindow", "0", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Map", nullptr));
        label_lat->setText(QApplication::translate("MainWindow", "Latitude:", nullptr));
        label_long->setText(QApplication::translate("MainWindow", "Longitude:", nullptr));
        label_lat_value->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_long_value->setText(QApplication::translate("MainWindow", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_cockpit), QApplication::translate("MainWindow", "Cockpit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
