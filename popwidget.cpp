#include "popwidget.h"
#include "ui_popwidget.h"
#include <QModelIndex>
#include <QSqlQuery>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include "windows.h"
#include "handledialog.h"
#include "globdata.h"
#include <math.h>

PopWidget::PopWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopWidget)
{
    ui->setupUi(this);
    ui->handlePushButton->setObjectName("cButton");
    setWindowFlags(Qt::Dialog | Qt::Popup | Qt::FramelessWindowHint);
    this->setStyleSheet("PopWidget{background-color:rgb(254,0,127);}");
    ui->pushButton->setButtonStyle(":/image/toolbar_close.png",4);
    setFixedHeight(200);
}

PopWidget::~PopWidget()
{
    delete ui;
}

void PopWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PopWidget::showEvent(QShowEvent *event)
{
    int width = GetSystemMetrics(SM_CXFULLSCREEN);
    int height = GetSystemMetrics(SM_CYFULLSCREEN);
    move(width - rect().width(),height-rect().height()+20);
}

void PopWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    return;
}

void PopWidget::addIndex(int alarm_key)
{
    QString sql = QString("SELECT zone_num,zone_name,part_num,part_name,event_time,zone_type FROM alarm WHERE alarm_main_key = %1").arg(alarm_key);
    QSqlQuery query;
    query.exec(sql);
    if(query.next())
    {
        indexStack.push(alarm_key);
        QString alrmType = query.value(5).toString();
        //qDebug() <<"警情句柄：" << alarm_key << endl;
        ui->alarmNumLineEdit->setText(query.value(0).toString());
        ui->timeLineEdit->setText(query.value(4).toString());
        ui->alarmTypeLineEdit->setText(alrmType);
        ui->alarmNameLineEdit->setText(query.value(1).toString());
        ui->partNumLineEdit->setText(query.value(2).toString());
        ui->partNameineEdit->setText(query.value(3).toString());
        ui->warmLabel->setText(QString("%1警情待处理").arg(indexStack.count()));
    }
    this->show();

}



void PopWidget::on_handlePushButton_clicked()
{
    int key = indexStack.top();
    QString sql = QString("SELECT charg1_name,charg1_phone,charg2_name,charg3_phone FROM alarm WHERE alarm_main_key = %1").arg(key);
    QSqlQuery query;
    query.exec(sql);
    if(query.next())
    {
        QString n1= query.value(0).toString();
        QString p1= query.value(1).toString();
        QString n2= query.value(2).toString();
        QString p2= query.value(3).toString();
        HandleDialog dialog;
        dialog.setData(p1,p1,p2,p2);
        if(dialog.exec() == QDialog::Accepted)
        {
            indexStack.pop();
            HandleDialogData *data = dialog.getdata();
            sql = QString("UPDATE alarm SET is_handle = %1,handle_person = '%2',handle_opinion = '%3'"
                          ",notice_person = '%4',notice_phone='%5' WHERE alarm_main_key = %6 ").arg(1)
                    .arg(glob_nickName).arg(data->alarmTypeString).arg(data->contackPeople).arg(data->contackPhone).arg(key);
            query.exec(sql);
            model->setFilter(model->filter());
            model->select();
        }
    }
    if(indexStack.count() != 0)
    {
       ui->warmLabel->setText(QString("%1警情待处理").arg(indexStack.count()));
       key = indexStack.top();
       sql = QString("SELECT zone_num,zone_name,part_num,part_name,event_time,zone_type FROM alarm WHERE alarm_main_key = %1").arg(key);
       query.exec(sql);
       if(query.next())
       {
           QString alrmType = query.value(5).toString();
           //qDebug() <<"警情句柄：" << alarm_key << endl;
           ui->alarmNumLineEdit->setText(query.value(0).toString());
           ui->timeLineEdit->setText(query.value(4).toString());
           ui->alarmTypeLineEdit->setText(alrmType);
           ui->alarmNameLineEdit->setText(query.value(1).toString());
           ui->partNumLineEdit->setText(query.value(2).toString());
           ui->partNameineEdit->setText(query.value(3).toString());
       }
    }
    else
    {
        this->hide();
    }
}

void PopWidget::on_pushButton_clicked()
{
    indexStack.clear();
    hide();
}
