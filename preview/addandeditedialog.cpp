#include "addandeditedialog.h"
#include "ui_addandeditedialog.h"
#include "deviceform.h"
#include "channeleditdialog.h"
#include "globdata.h"
#include<QSqlRecord>
#include<QSqlQuery>
#include<QMessageBox>

enum {
    SERIAL,
    MODE,
    NAME,
    IP,
    PORT,
    ACCOUNT,
    PASSWARD,
    TYPE,
    CHANNAL_AMOUNT,
    LINE_STATE
};

AddAndEditeDialog::AddAndEditeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAndEditeDialog),
    model(new DeviceImforSqlTableModel(this)),
    channalModel(new QSqlTableModel(this))
{
    ui->setupUi(this);
    this->setWindowTitle("信息编辑");
    this->setFixedSize(600,450);
    setAttribute(Qt::WA_DeleteOnClose);
    model->setTable("deviceImform");
    model->select();
    model->setHeaderData(MODE,Qt::Horizontal,"设备厂家");//海康，大华
    model->setHeaderData(NAME,Qt::Horizontal,"设备名称");
    model->setHeaderData(TYPE,Qt::Horizontal,"设备类型");//NVR,IPC
    model->setHeaderData(CHANNAL_AMOUNT,Qt::Horizontal,"通道数");
    model->setHeaderData(LINE_STATE,Qt::Horizontal,"在线状态");

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(SERIAL,true);
    ui->tableView->setColumnHidden(PORT,true);
    ui->tableView->setColumnHidden(ACCOUNT,true);
    ui->tableView->setColumnHidden(PASSWARD,true);
    ui->tableView->setColumnHidden(LINE_STATE,true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setCurrentIndex(model->index(0,0));
    ui->tableView->verticalHeader()->setVisible(false);
    channalModel->setTable("channelName");
    channalModel->setHeaderData(1,Qt::Horizontal,"通道号");
    channalModel->setHeaderData(2,Qt::Horizontal,"通道名称");

    ui->channalTableView->setModel(channalModel);
    ui->channalTableView->setColumnHidden(0,true);
    ui->channalTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->channalTableView->horizontalHeader()->setStretchLastSection(true);//最后一列填充
    ui->channalTableView->verticalHeader()->setVisible(false);
    updateChannalTableView();
    connect(ui->tableView->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),
            this,SLOT(updateChannalTableView()));
}

AddAndEditeDialog::~AddAndEditeDialog()
{
    delete ui;
}

void AddAndEditeDialog::on_editPushButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    DeviceForm *form = new DeviceForm(this,model,row);
    form->setWindowTitle("编辑设备");
    form->show();
}

void AddAndEditeDialog::on_addPushButton_clicked()
{
    DeviceForm *form = new DeviceForm(this,model,-1);
    form->setWindowTitle("添加设备");
    form->show();
}

void AddAndEditeDialog::on_delePushButton_clicked()
{
    if(!ui->tableView->currentIndex().isValid())
    {
        QMessageBox::warning(this,"提醒","请选中要删除的设备");
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("确认要删除此设备吗");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    int row = ui->tableView->currentIndex().row();
    int serial = model->data(model->index(row,SERIAL),Qt::EditRole).toInt();

    //删除通道名称表里的关于本录像机的数据
    QSqlQuery query;
    query.exec(QString("DELETE FROM channelName WHERE serial = %1").arg(serial));
    model->removeRow(row);
    model->submitAll();
    model->select();
    QSqlDatabase::database().commit();
    updateChannalTableView();
    //左边设备树删除这个设备
    treeModle->removeItem(serial);
}

void AddAndEditeDialog::updateChannalTableView()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid())
    {
        QSqlRecord record = model->record(index.row());
        int serial = record.value("serial").toInt();
        int n = record.value("channel_amount").toInt();
        QString filter;
        if(n == 1)
        {
            filter = QString("serial = %1 AND channel_number < %2").arg(serial).arg(n);
            ui->editChannalPushButton->setEnabled(false);
        }
        else {
           filter = QString("serial = %1 AND channel_number <= %2").arg(serial).arg(n);
           ui->editChannalPushButton->setEnabled(true);
        }
        channalModel->setFilter(filter);
    }
    else {
            QString filter = QString("serial = -1");
            channalModel->setFilter(filter);
    }
    channalModel->select();
    ui->channalTableView->horizontalHeader()->setVisible(channalModel->rowCount() > 0);
}

void AddAndEditeDialog::on_editChannalPushButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid())
    {
        QSqlRecord record = model->record(index.row());
        int serial = record.value("serial").toInt();
        int n = record.value("channel_amount").toInt();
        ChannelEditDialog *dialog = new ChannelEditDialog(this,serial,n,channalModel);
        dialog->exec();
    }

}

void AddAndEditeDialog::on_quitPushButton_clicked()
{
    this->close();
}
