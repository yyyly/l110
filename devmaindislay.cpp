#include "devmaindislay.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>

DevMainDislay::DevMainDislay(QWidget *parent)
    : QWidget(parent),
      devStatus(new DocumetDisplay(this)),
      relayLabel_1(new QLabel("继电器1")),
      relayLabel_2(new QLabel("继电器2")),
      relaySwitch_1(new SwitchButton()),
      relaySwitch_2(new SwitchButton()),
      voice(new QLabel("声   音")),
      voiceSWitch(new SwitchButton()),
      alarm(new QPushButton("布防")),
      disAlarm(new QPushButton("撤防")),
      clearMark(new QPushButton("消警"))
{
    setAutoFillBackground(true);
    alarm->setObjectName("woodButton");
    disAlarm->setObjectName("woodButton");
    clearMark->setObjectName("woodButton");
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    QHBoxLayout *hlayout_relay = new QHBoxLayout();
    hlayout1->addWidget(relayLabel_1);
    hlayout1->addWidget(relaySwitch_1);
    hlayout2->addWidget(relayLabel_2);
    hlayout2->addWidget(relaySwitch_2);
    hlayout_relay->addLayout(hlayout1);
    hlayout_relay->addStretch();
    hlayout_relay->addLayout(hlayout2);
    QHBoxLayout *hlayout_voice = new QHBoxLayout();
    hlayout_voice->addWidget(voice);
    hlayout_voice->addWidget(voiceSWitch);
    hlayout_voice->addStretch();
    QHBoxLayout *hlayout3 = new QHBoxLayout();
    hlayout3->addWidget(alarm);
    hlayout3->addWidget(disAlarm);
    hlayout3->addWidget(clearMark);
    QVBoxLayout *Vlayout = new QVBoxLayout();
    Vlayout->addWidget(devStatus);
    Vlayout->addSpacing(20);
    Vlayout->addLayout(hlayout3);
    Vlayout->addLayout(hlayout_voice);
    Vlayout->addLayout(hlayout_relay);

    Vlayout->setContentsMargins(0,0,0,10);
    hlayout_relay->setContentsMargins(5,0,5,0);
    hlayout_voice->setContentsMargins(5,0,0,0);
    hlayout3->setContentsMargins(5,0,5,0);
    setLayout(Vlayout);
}

void DevMainDislay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
