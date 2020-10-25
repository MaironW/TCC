#ifndef ATTITUDEINDICATOR_H
#define ATTITUDEINDICATOR_H

#include <QWidget>

class AttitudeIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit AttitudeIndicator(QWidget *parent = nullptr);
    void setAttitude(QVector3D* euler);

private:
    QVector3D* attitude;

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
};

#endif // ATTITUDEINDICATOR_H
