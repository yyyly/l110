#include "switchbutton.h"
#include<QPainter>

SwitchButton::SwitchButton(QWidget *parent)
    : QWidget(parent),
      statu(Statu::OFF)
{
    //setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
    onPixmap = QPixmap(":/image/switch_on.png");
    offPixmap = QPixmap(":/image/switch_off.png");
}

QSize SwitchButton::sizeHint() const
{
   QSize size = onPixmap.size();
   QSize s = size/5*2;
   return s;
}

QSize SwitchButton::maximumSize() const
{
    QSize size = onPixmap.size();
    QSize s = size/5*2;
    return s;
}

QSize SwitchButton::minimumSize() const
{
    QSize size = onPixmap.size();
    QSize s = size/5*2;
    return s;
}

void SwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(statu == Statu::ON)
    {
        painter.drawPixmap(0,0,onPixmap.scaled(size()));
    }
    else
    {
        painter.drawPixmap(0,0,offPixmap.scaled(size()));
    }
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(statu == Statu::OFF)
        {
            statu = Statu::ON;
            update();
        }
        else
        {
            statu = Statu::OFF;
            update();
        }
        emit statuChanged(statu);
        event->accept();
    }

}

Statu SwitchButton::getStatu()
{
    return statu;
}

void SwitchButton::setStatu(Statu s)
{
    statu = s;
}
