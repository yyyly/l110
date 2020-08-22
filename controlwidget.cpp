#include "controlwidget.h"
#include "ui_controlwidget.h"
#include <QModelIndexList>
#include <QSqlRecord>
#include "handledialog.h"
#include "globdata.h"


enum{
    ZONE_NUM,
    ZONE_NAME,
    PART_NUM,
    PART_NAME,
    ZONE_TYPE,
    EVENT_TIME,
    REPORT_TIME,
    CHARG1_NAME,
    CHARG1_PHONE,
    CHARG2_NAME,
    CHARG2_PHONE,
    IS_HANDLE,
    HANDLE_PERSON,
    HANDLE_OPINION,
    NOTICE_PERSON,
    NOTICE_PHONE,
    ALARM_CLASS,
    VEDIO_PATH
};

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget),
    model(nullptr)
{
    ui->setupUi(this);
    ui->handalPushButton->setObjectName("cButton");
    ui->fastHandlePushButton->setObjectName("cButton");
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::on_alarmWarmingCheckBox_stateChanged(int arg1)
{
    if(!arg1)//未选中，不显示报警信息
        {
            if(ui->deviceSystemcheckBox->isChecked())//系统信息显示
            {
                state = 1;

            }
            else {//都未选中，表示显示所有信息
                state = 2;
            }
        }
    else {//显示报警信息
        if(ui->deviceSystemcheckBox->isChecked())
        {
            state = 2;
        }
        else {
            state = 0;
        }

    }
    emit stateChange(state);

}

void ControlWidget::on_deviceSystemcheckBox_stateChanged(int arg1)
{
    if(!arg1)
    {
        if(ui->alarmWarmingCheckBox->isChecked())
        {
            state = 0;
        }
        else {
            state = 2;
        }
    }
    else {
        if(ui->alarmWarmingCheckBox->isChecked())
        {
            state = 2;
        }
        else {
            state = 1;
        }
    }
    emit stateChange(state);
}

void ControlWidget::on_handalPushButton_clicked()
{
    QModelIndex index = view->currentIndex();
    QSqlRecord record = model->record(index.row());
    QString p1 = record.value(CHARG1_NAME).toString();
    QString p2 = record.value(CHARG2_NAME).toString();
    QString ph1 = record.value(CHARG1_PHONE).toString();
    QString ph2 = record.value(CHARG2_PHONE).toString();
    HandleDialog dialog;
    dialog.setData(p1,ph1,p2,ph2);
    if(dialog.exec() == QDialog::Accepted)
    {
        HandleDialogData *data = dialog.getdata();
        if(data)
        {
            model->setData(model->index(index.row(),IS_HANDLE),1);
            model->setData(model->index(index.row(),NOTICE_PERSON),data->contackPeople);
            model->setData(model->index(index.row(),NOTICE_PHONE),data->contackPhone);
            model->setData(model->index(index.row(),HANDLE_OPINION),data->alarmTypeString);
            model->submit();
        }
        if(state == 0)
        {
           model->setFilter("(IS_HANDLE != 1 OR IS_HANDLE IS null) AND ALARM_CLASS == 0");
        }
        else if(state ==1)
        {
           model->setFilter("(IS_HANDLE != 1 OR IS_HANDLE IS null) AND ALARM_CLASS == 1");
        }
        else {
            model->setFilter("IS_HANDLE != 1 OR IS_HANDLE IS null");
        }

        model->select();
    }
}

void ControlWidget::on_fastHandlePushButton_clicked()
{
    if(view == nullptr)
        return;
    if(model == nullptr)
        return;
    /*int row  = view->currentIndex().row();
    model->setData(model->index(row,IS_HANDLE),1);//1已处理
    model->submit();
    model->setFilter("IS_HANDLE != 1 OR IS_HANDLE IS null");
    model->select();
    emit handleHappened();*/
    QModelIndexList indexList = view->selectionModel()->selectedRows();
    QModelIndex index;
    foreach (index, indexList) {
        int row  = index.row();
        model->setData(model->index(row,IS_HANDLE),1);
        model->setData(model->index(row,HANDLE_PERSON),glob_nickName);
        model->setData(model->index(row,HANDLE_OPINION),"快速处理");
        model->submit();
    }
    if(state == 0)
    {
       model->setFilter("(IS_HANDLE != 1 OR IS_HANDLE IS null) AND ALARM_CLASS == 0");
    }
    else if(state ==1)
    {
       model->setFilter("(IS_HANDLE != 1 OR IS_HANDLE IS null) AND ALARM_CLASS == 1");
    }
    else {
        model->setFilter("IS_HANDLE != 1 OR IS_HANDLE IS null");
    }

    model->select();
}
