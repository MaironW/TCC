#ifndef SELECTPORTWINDOW_H
#define SELECTPORTWINDOW_H

#include <QDialog>

namespace Ui {
class SelectPortWindow;
}

class SelectPortWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPortWindow(QWidget *parent = nullptr);
    ~SelectPortWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SelectPortWindow *ui;
};

#endif // SELECTPORTWINDOW_H
