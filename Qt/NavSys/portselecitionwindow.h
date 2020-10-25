#ifndef PORTSELECITIONWINDOW_H
#define PORTSELECITIONWINDOW_H

#include <QDialog>

namespace Ui {
class PortSelecitionWindow;
}

class PortSelecitionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PortSelecitionWindow(QWidget *parent = nullptr);
    ~PortSelecitionWindow();

    bool USB;

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PortSelecitionWindow *ui;

};

#endif // PORTSELECITIONWINDOW_H
