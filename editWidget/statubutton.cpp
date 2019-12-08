#include "statubutton.h"
#include <QStyleOption>
#include <QPainter>

StatuButton::StatuButton(QWidget *parent)
    :QPushButton(parent)
{
    SetState(0);
    connect(this,SIGNAL(click()),this,SLOT(updateState()));
    this->setText("星期1");
}

void StatuButton::SetState(int s)
{
    state = s;
    if(s == 0)
    {
        setStyleSheet("QPushButton{background-color: red;color: white;}");
    }
}

void StatuButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOptionButton opt;
    opt.init(this);
    QPainter p(this);
    QPalette palette(this->palette());
    style()->drawControl(QStyle::CE_PushButton, &opt, &p, this);
    style()->drawItemText(&p,opt.rect,Qt::AlignCenter,palette,true,this->text());
}

void StatuButton::updateState()
{
    if(state == 1)
    {
        SetState(0);
    }
    else {
        SetState(1);
    }
}
