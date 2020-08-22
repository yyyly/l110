#include "icondeswidget.h"
#include <QPaintEvent>
#include <QPainter>

IconDesWidget::IconDesWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(45);
    setFixedWidth(200);
}

void IconDesWidget::paintEvent(QPaintEvent *event)
{
    QRectF rect = event->rect();
    QPainter p(this);
    p.save();
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(243,243,243));
    p.drawRect(rect);
    p.restore();
    QRectF imageRect = QRectF(6,6,32,32);
    p.drawImage(imageRect,QImage(":/image/defence.png"));
    p.drawText(rect- QMargins(36,16,0,0),"布防");
    imageRect = QRectF(57,6,32,32);
    p.drawImage(imageRect,QImage(":/image/outDefence.png"));
    p.drawText(rect- QMargins(87,16,0,0),"撤防");
    imageRect = QRectF(108,6,32,32);
    p.drawImage(imageRect,QImage(":/image/noStatu.png"));
    p.drawText(rect- QMargins(138,16,0,0),"未知");

}
