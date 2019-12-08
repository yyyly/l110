#include "ptzcontrol.h"
#include "ui_ptzcontrol.h"
#include<QStyleOption>
#include<QPainter>
#include<QPalette>

PTZControl::PTZControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PTZControl)
{
    ui->setupUi(this);
    setObjectName("ptz");
    ui->ptzButton_B->installEventFilter(this);
    ui->ptzButton_L->installEventFilter(this);
    ui->ptzButton_R->installEventFilter(this);
    ui->ptzButton_T->installEventFilter(this);
}

PTZControl::~PTZControl()
{
    delete ui;
}

void PTZControl::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool PTZControl::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(watched == ui->ptzButton_B)
        {
            emit toDerection(Camero::DOWN,0);
        }
        else if(watched == ui->ptzButton_L)
        {
            emit toDerection(Camero::LEFT,0);
        }
        else if(watched == ui->ptzButton_R)
        {
            emit toDerection(Camero::RIGHT,0);
        }
        else
        {
            emit toDerection(Camero::UP,0);
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        if(watched == ui->ptzButton_B)
        {
            emit toDerection(Camero::DOWN,1);
        }
        else if(watched == ui->ptzButton_L)
        {
            emit toDerection(Camero::LEFT,1);
        }
        else if(watched == ui->ptzButton_R)
        {
            emit toDerection(Camero::RIGHT,1);
        }
        else
        {
            emit toDerection(Camero::UP,1);
        }
    }
    return false;
}
