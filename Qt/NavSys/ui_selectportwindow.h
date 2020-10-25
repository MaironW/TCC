/********************************************************************************
** Form generated from reading UI file 'selectportwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTPORTWINDOW_H
#define UI_SELECTPORTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectPortWindow
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *SelectPortWindow)
    {
        if (SelectPortWindow->objectName().isEmpty())
            SelectPortWindow->setObjectName(QStringLiteral("SelectPortWindow"));
        SelectPortWindow->resize(292, 48);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SelectPortWindow->sizePolicy().hasHeightForWidth());
        SelectPortWindow->setSizePolicy(sizePolicy);
        SelectPortWindow->setMinimumSize(QSize(292, 48));
        SelectPortWindow->setMaximumSize(QSize(292, 48));
        widget = new QWidget(SelectPortWindow);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 271, 27));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        retranslateUi(SelectPortWindow);

        QMetaObject::connectSlotsByName(SelectPortWindow);
    } // setupUi

    void retranslateUi(QDialog *SelectPortWindow)
    {
        SelectPortWindow->setWindowTitle(QApplication::translate("SelectPortWindow", "Select communication port", nullptr));
        pushButton_2->setText(QApplication::translate("SelectPortWindow", "USB", nullptr));
        pushButton->setText(QApplication::translate("SelectPortWindow", "Bluetooth", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectPortWindow: public Ui_SelectPortWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTPORTWINDOW_H
