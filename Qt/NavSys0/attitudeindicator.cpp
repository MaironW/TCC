#include "attitudeindicator.h"

#include <QDebug>
#include <QtWidgets>

AttitudeIndicator::AttitudeIndicator(QWidget *parent) : QWidget(parent)
{

}

void AttitudeIndicator::paintEvent(QPaintEvent *event){
    // bank indicator
    static const QPoint rollHand[3] = {
        QPoint(9, -65),
        QPoint(-9, -65),
        QPoint(0, -85)
    };

    static const QPoint rollHandDown[3] = {
        QPoint(9, -105),
        QPoint(-9, -105),
        QPoint(0, -85)
    };

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 102, 255);
    QColor groundColor(180, 107, 39);

    int side = qMin(width(), height());

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 250.0, side / 250.0);

    painter.setPen(Qt::NoPen);

    painter.setBrush(minuteColor);

    painter.save();

    QRectF rectangle(-width()*1.5,-height()*1.5,side*3,side*3);

    // draw background
    painter.rotate(attitude->x());
    painter.translate(0,attitude->y()*2.5);

    painter.setBrush(minuteColor);
    painter.drawPie(rectangle, 0*16 ,180*16);   // draw sky
    painter.setBrush(groundColor);
    painter.drawPie(rectangle, 0*16 , -180*16); // draw ground

    // pitch scale
    painter.setPen(QPen(Qt::white,2));
    painter.drawLine(-width()*1.5,0,width()*1.5,0);
    painter.drawLine(-30, 10*2.5, 30, 10*2.5);
    painter.drawLine(-30, -10*2.5, 30, -10*2.5);
    painter.drawLine(-35, 20*2.5, 35, 20*2.5);
    painter.drawLine(-35, -20*2.5, 35, -20*2.5);
    painter.drawLine(-10, -5*2.5, 10, -5*2.5);
    painter.drawLine(-10, 5*2.5, 10, 5*2.5);
    painter.drawLine(-10, -15*2.5, 10, -15*2.5);
    painter.drawLine(-10, 15*2.5, 10, 15*2.5);

    painter.restore();
    painter.save();

    // draw roll indicator
    painter.setPen(QPen(Qt::yellow,2));
    painter.rotate(attitude->x());
    painter.setBrush(Qt::NoBrush);
    painter.drawConvexPolygon(rollHand, 3);
    painter.restore();

    // draw zero bank indicator
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawConvexPolygon(rollHandDown, 3);

    // draw lines
    painter.setPen(QPen(Qt::white,2));
    painter.rotate(0);
    for (int i = 0; i < 7; ++i) {
        painter.drawLine(88, 0, 100, 0);
        painter.rotate(-30);
    }
    painter.rotate(-150);
    painter.rotate(-90);

    // draw small lines
    painter.rotate(10);
    painter.drawLine(88, 0, 96, 0);
    painter.rotate(10);
    painter.drawLine(88, 0, 96, 0);
    painter.rotate(25);
    painter.drawLine(88, 0, 90, 0);
    painter.rotate(-55);
    painter.drawLine(88, 0, 96, 0);
    painter.rotate(-10);
    painter.drawLine(88, 0, 96, 0);
    painter.rotate(-25);
    painter.drawLine(88, 0, 90, 0);
    painter.rotate(135);

    // draw wings
    painter.setPen(QPen(Qt::yellow,4));
    painter.drawLine(-1, 0, 1, 0);
    // right wing
    painter.drawLine(25, 0, 70, 0);
    painter.drawLine(25, 0, 25, 3);
    // left wing
    painter.drawLine(-25, 0, -70, 0);
    painter.drawLine(-25, 0, -25, 3);

}

void AttitudeIndicator::setAttitude(QVector3D *euler){
    attitude = euler;
}
