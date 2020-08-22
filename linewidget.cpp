#include "linewidget.h"
#include <QStyleOption>
#include <QPainter>

LineWidget::LineWidget(QWidget *parent) : QWidget(parent)
{

}

void LineWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setPen(QPen(Qt::gray,4));
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(2,2,width() -4,height()-4,0,0);
}
