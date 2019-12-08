#include "devpanel.h"
#include<QStyleOption>
#include<QPainter>
#include<QSizePolicy>
#include"style/norwegianwoodstyle.h"
DevPanel::DevPanel(QMap<QString, QVariant> *data, QWidget *parent)
    : QFrame(parent),
      title(new QLabel()),
      leftButton(new QPushButton()),
      rightButton(new QPushButton()),
      index(0),
      mainDisplay(new DevMainDislay()),
      hLayout(new QHBoxLayout()),
      vLayout(new QVBoxLayout(this)),
      d(data)
{
    setFixedSize(280,300);
    setObjectName("devPanel");
    title->setFixedSize(280,20);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("title");
    leftButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    leftButton->setObjectName("leftButton");
    rightButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    rightButton->setObjectName("rightButton");
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,10,0,0);
    currentWidget = centroWidget[0] = mainDisplay;
    for(int i = 1;i < 10;i++)
    {
        partDisplay = new DevPartDisplay();
        centroWidget[i] = partDisplay;
    }
    connect(leftButton,SIGNAL(clicked(bool)),this,SLOT(leftButtonClick()));
    connect(rightButton,SIGNAL(clicked(bool)),this,SLOT(rightButtonClick()));
    setStyle(new NorwegianWoodStyle());
}



void DevPanel::haveNewData(MessingType type, QMap<QString, QVariant> *data)
{
    if(!data)
        return;
    QString sty;

    if(d[0].value("DevStatus") == DeviceStatus::DisLogin)
    {
        sty = styleSheet();
        setStyleSheet("DevPanel{background:url(:/image/panel.png) no-repeat;}");
        return;
    }
    switch (type) {
    case MessingType::Login:
    {
        setStyleSheet(sty);
        title->setText(d[0].value("title").toString());
        hLayout->addWidget(leftButton);
        hLayout->addWidget(centroWidget[0]);
        hLayout->addWidget(rightButton);
        vLayout->addWidget(title);
        vLayout->addLayout(hLayout);
    }
        break;
    case MessingType::PartImfor:
    {
        int n = data->value("partN").toInt();
        QString str = d[n].value("areas").toString();
        if(str.isEmpty())
        {
            return;
        }
        DevPartDisplay *w = static_cast<DevPartDisplay *>(centroWidget[n]);
        w->partAreaChange(str);
    }
        break;
    default:
        break;
    }

}


void DevPanel::leftButtonClick()
{
    if(index == 0)
        return;
    lastWidget = centroWidget[index--];
    title->setText(d[index].value("title").toString());
    currentWidget = centroWidget[index];
    hLayout->removeWidget(lastWidget);
    lastWidget->hide();
    currentWidget->show();
    hLayout->insertWidget(1,currentWidget);

}

void DevPanel::rightButtonClick()
{
    if(index ==8)
        return;
    lastWidget = centroWidget[index++];
    title->setText(d[index].value("title").toString());
    currentWidget = centroWidget[index];
    hLayout->removeWidget(lastWidget);
    lastWidget->hide();
    currentWidget->show();
    hLayout->insertWidget(1,currentWidget);
}

void DevPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
