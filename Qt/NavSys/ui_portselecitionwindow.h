/********************************************************************************
** Form generated from reading UI file 'portselecitionwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PORTSELECITIONWINDOW_H
#define UI_PORTSELECITIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_PortSelecitionWindow
{
public:
    QDialogButtonBox *buttonBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;

    void setupUi(QDialog *PortSelecitionWindow)
    {
        if (PortSelecitionWindow->objectName().isEmpty())
            PortSelecitionWindow->setObjectName(QStringLiteral("PortSelecitionWindow"));
        PortSelecitionWindow->resize(232, 73);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PortSelecitionWindow->sizePolicy().hasHeightForWidth());
        PortSelecitionWindow->setSizePolicy(sizePolicy);
        PortSelecitionWindow->setMinimumSize(QSize(232, 73));
        PortSelecitionWindow->setMaximumSize(QSize(232, 73));
        PortSelecitionWindow->setBaseSize(QSize(0, 0));
        buttonBox = new QDialogButtonBox(PortSelecitionWindow);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(140, 30, 81, 31));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        radioButton = new QRadioButton(PortSelecitionWindow);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(10, 10, 112, 23));
        radioButton->setChecked(true);
        radioButton_2 = new QRadioButton(PortSelecitionWindow);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(10, 30, 112, 23));

        retranslateUi(PortSelecitionWindow);
        QObject::connect(buttonBox, SIGNAL(rejected()), PortSelecitionWindow, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), PortSelecitionWindow, SLOT(accept()));

        QMetaObject::connectSlotsByName(PortSelecitionWindow);
    } // setupUi

    void retranslateUi(QDialog *PortSelecitionWindow)
    {
        PortSelecitionWindow->setWindowTitle(QApplication::translate("PortSelecitionWindow", "Select communication port", nullptr));
        radioButton->setText(QApplication::translate("PortSelecitionWindow", "USB", nullptr));
        radioButton_2->setText(QApplication::translate("PortSelecitionWindow", "Bluetooth", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PortSelecitionWindow: public Ui_PortSelecitionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTSELECITIONWINDOW_H
