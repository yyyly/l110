#include "channeleditdialog.h"
#include "ui_channeleditdialog.h"
#include<QSqlQuery>
#include<QSqlRecord>
#include "globdata.h"

ChannelEditDialog::ChannelEditDialog(QWidget *parent, int s, int n,QSqlTableModel *m) :
    QDialog(parent),
    ui(new Ui::ChannelEditDialog),
    serial(s),
    num(n),
    model(m)
{
    ui->setupUi(this);
    for(int i = 1;i <= n;i++)
    {
        ui->comboBox->addItem(QString("%1").arg(i));
    }
    setAttribute(Qt::WA_DeleteOnClose);
    QSqlQuery query;
    query.exec(QString("SELECT channel_name FROM channelName WHERE serial = %1"
                       " AND channel_number = %2").arg(serial).arg(1));
    if(query.next())
    {
        QString channelName = query.value(0).toString();
        ui->lineEdit->setText(channelName);
    }
}

ChannelEditDialog::~ChannelEditDialog()
{
    delete ui;
}

void ChannelEditDialog::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
        return;
    int number = ui->comboBox->currentIndex() + 1;//通到编号
    //查询数据库中序列号为serial的设备是否未通道number命名，如果有命名则修改名称，
    //否则新添加一行
    QSqlRecord record;
    for(int i = 0;i < model->rowCount();i++)
    {
        record = model->record(i);
        if(record.value(1).toInt() == number)
        {
            //修改名称
            model->setData(model->index(i,2),ui->lineEdit->text());
            if(model->submit())
            {
                //回应“更新成功”//
            }
            //修改设备树的显示
            QStandardItem *item = treeModle->findItem(serial,number);
            item->setData(ui->lineEdit->text(),Qt::EditRole);
            return;
        }
    }
    model->insertRows(0,1);
    model->setData(model->index(0,0),serial);
    model->setData(model->index(0,1),number);
    model->setData(model->index(0,2),ui->lineEdit->text());
    if(model->submit())
    {
        //回应“添加成功”//
    }
    QString filter = QString("serial = %1 AND channel_number <= %2").arg(serial).arg(num);
    model->setFilter(filter);
    model->select();
    QStandardItem *item = treeModle->findItem(serial,number);
    item->setData(ui->lineEdit->text(),Qt::EditRole);
    //修改结果是否要写入全局变量？
}

void ChannelEditDialog::on_comboBox_currentIndexChanged(int index)
{
    QSqlQuery query;
    query.exec(QString("SELECT channel_name FROM channelName WHERE serial = %1"
                       " AND channel_number = %2").arg(serial).arg(index + 1));
    if(query.next())
    {
        QString channelName = query.value(0).toString();
        ui->lineEdit->setText(channelName);
    }
    else
    {
        ui->lineEdit->setText("");
    }
}
