#include "cstatubar.h"
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QPropertyAnimation>

const int lineWidth = 1;

CStatuBar::CStatuBar(QWidget *parent)
    : QWidget(parent),
      timer(new QTimer(this))
{
    connect(timer,SIGNAL(timeout()),this,SLOT(HideWidget()));
}

void CStatuBar::showMessage(const QString str, int time)
{
    message = str;
    timer->start(time);//ms
    this->show();
}


void CStatuBar::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    setFixedSize(160,40);
    setWindowOpacity(0);
}

void CStatuBar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setPen(QPen(Qt::black,lineWidth));
    QLinearGradient gradient(50,100,300,350);
    gradient.setColorAt(0.0,Qt::white);
    gradient.setColorAt(0.2,Qt::green);
    gradient.setColorAt(1.0,Qt::black);
    p.setBrush(gradient);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(1,1,width()-(2*lineWidth) - 1,height()-(2 * lineWidth),0,0);
    //绘制文字
    QFont font("楷体",10,QFont::Bold,true);
    p.setFont(font);
    p.setPen(Qt::black);
    p.drawText(event->rect(),Qt::AlignCenter,message);

}

void CStatuBar::HideWidget()
{
    /*QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(hide()));*/
    hide();
}
