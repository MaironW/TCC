#include "compass.h"

#include <QDebug>
#include <QtWidgets>

Compass::Compass(QWidget *parent) : QWidget(parent)
{

}

void Compass::paintEvent(QPaintEvent *){

    static const QPoint yawHand[3] = {
        QPoint(7,8),
        QPoint(-7,8),
        QPoint(0,-70)
    };

    QColor hourColor(204,0,0);
    QColor minuteColor(0,102,255);

    int side = qMin(width(),height());

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);
    painter.save();
    painter.rotate(0);
    painter.drawConvexPolygon(yawHand, 3);
    painter.restore();

    painter.setPen(hourColor);
    painter.save();
    painter.rotate(0);
    painter.drawConvexPolygon(yawHand,0);
    painter.restore();


    painter.setPen(hourColor);
    painter.rotate(yaw);
    for(int i = 0; i < 12; i++){
        painter.drawLine(88,0,96,0);
        painter.rotate(30);
    }

    QFont font=painter.font();
    font.setPointSize(6);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);

    painter.rotate(30);
    painter.drawText(QPoint(-9,-70),"30");
    painter.rotate(30);
    painter.drawText(QPoint(-9,-70),"60");
    painter.rotate(60);
    painter.drawText(QPoint(-9,-70),"120");
    painter.rotate(30);
    painter.drawText(QPoint(-9,-70),"150");
    painter.rotate(60);
    painter.drawText(QPoint(-9,-70),"-150");
    painter.rotate(30);
    painter.drawText(QPoint(-9,-70),"-120");
    painter.rotate(60);
    painter.drawText(QPoint(-9,-70),"-60");
    painter.rotate(30);
    painter.drawText(QPoint(-9,-70),"-30");
    painter.rotate(30);


    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }

    painter.setPen(hourColor);
    font.setPointSize(12);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);

    painter.drawText(QPoint(-9,-70),"N");
    painter.rotate(90);
    painter.drawText(QPoint(-9,-70),"E");
    painter.rotate(90);
    painter.drawText(QPoint(-9,-70),"S");
    painter.rotate(90);
    painter.drawText(QPoint(-9,-70),"W");

}

void Compass::setHeading(double heading){
    yaw = heading;
}
