#ifndef COMPASS_H
#define COMPASS_H

#include <QWidget>

class Compass : public QWidget
{
    Q_OBJECT
public:
    explicit Compass(QWidget *parent = nullptr);
    double heading;
    void setHeading(double heading);

protected:
    void paintEvent(QPaintEvent *event) override;
    double yaw;

signals:

public slots:
};

#endif // COMPASS_H
