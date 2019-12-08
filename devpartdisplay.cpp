#include "devpartdisplay.h"
#include<QLabel>

#include<QStyleOption>
#include<QPainter>
#include<QPaintEvent>

DevPartDisplay::DevPartDisplay(QWidget *parent)
    :QWidget(parent),
      widget(new QWidget(this)),
      alarmButton(new QPushButton("布防")),
      disAlarmButton(new QPushButton("撤防")),
      alarmIndicator(new Indicator("",IndicatorStatu::ALARM)),
      disAlarmIndictor(new Indicator("",IndicatorStatu::DISALARM)),
      passIndictor(new Indicator("",IndicatorStatu::PASS))
{
    setObjectName("devpartdisplay");
    rLayout = new QGridLayout();
    vLayout = new QVBoxLayout();
    rLayout->setMargin(0);
    vLayout->setContentsMargins(0,5,0,0);
    vLayout->addLayout(rLayout);
    vLayout->addStretch();
    widget->setLayout(vLayout);
    widget->setObjectName("screen");
    widget->setFixedHeight(180);
    widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(alarmButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(disAlarmButton);
    QHBoxLayout *indicatorLayout = new QHBoxLayout();
    indicatorLayout->addWidget(alarmIndicator);
    indicatorLayout->addWidget(disAlarmIndictor);
    indicatorLayout->addWidget(passIndictor);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,5);
    layout->addWidget(widget);
    layout->addStretch();
    layout->addLayout(buttonLayout);
    layout->addLayout(indicatorLayout);
    setLayout(layout);
}
DevPartDisplay::DevPartDisplay(const QString &list, QWidget *parent)
    : QWidget(parent)
{
   partAreaChange(list);
}

void DevPartDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DevPartDisplay::partAreaChange(const QString &list)
{
    Indicator *icon;
    int i = 0;
    int j = 0;
    alarmList = list.split("/");
    foreach(QString n,alarmList)
    {
        icon = new Indicator(n,IndicatorStatu::DISALARM);
        icon->setAlignment(Qt::AlignCenter);
        icon->setObjectName("disAlarm");
        icon->setAutoFillBackground(true);
        alarmIconList.push_back(icon);
    }
    foreach(Indicator* icon,alarmIconList)
    {
        if(j == 5)
        {
            i++;
            j = 0;
        }
        rLayout->addWidget(icon,i,j++,1,1);
    }
}

Indicator::Indicator(const QString &n, IndicatorStatu s, QWidget *parent) :
    QLabel(parent),
    statu(s)
{
    this->setText(n);
    this->setFixedSize(30,30);
}

void Indicator::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QRectF rect(this->rect());
    switch (statu) {
    case IndicatorStatu::DISALARM:
    {

        p.setPen(QPen(Qt::green,1));
        p.setBrush(QBrush(Qt::green));
        p.drawEllipse(rect);
        p.setFont(QFont("pointSize",12));
        p.setPen(QPen(Qt::white,2));
        QRect rect(0,7,30,14);
        p.drawText(rect,Qt::AlignHCenter,QString(this->text()));
    }
        break;
    case IndicatorStatu::ALARM:
    {
        p.setPen(QPen(Qt::red,1));
        p.setBrush(QBrush(Qt::red));
        p.drawEllipse(rect);
        p.setFont(QFont("pointSize",12));
        p.setPen(QPen(Qt::white,2));
        QRect rect(0,7,30,14);
        p.drawText(rect,Qt::AlignHCenter,QString(this->text()));
    }
        break;
    case IndicatorStatu::PASS:
    {
        p.setPen(QPen(Qt::gray,1));
        p.setBrush(QBrush(Qt::gray));
        p.drawEllipse(rect);
        p.setFont(QFont("pointSize",12));
        p.setPen(QPen(Qt::white,2));
        QRect rect(0,7,30,14);
        p.drawText(rect,Qt::AlignHCenter,QString(this->text()));
    }
        break;
    default:
        break;
    }
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

