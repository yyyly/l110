#include "dataentrydialog.h"
#include "ui_dataentrydialog.h"
#include <QModelIndexList>

DataEntryDialog::DataEntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataEntryDialog)

{
    ui->setupUi(this);
    partModel.setTable("partImfor");
    partModel.setSort(0,Qt::AscendingOrder);
    partModel.setHeaderData(0,Qt::Horizontal,"分区编号",Qt::DisplayRole);
    partModel.setHeaderData(1,Qt::Horizontal,"分区名称",Qt::DisplayRole);
    partModel.setHeaderData(2,Qt::Horizontal,"负责人1姓名",Qt::DisplayRole);
    partModel.setHeaderData(3,Qt::Horizontal,"负责人1电话",Qt::DisplayRole);
    partModel.setHeaderData(4,Qt::Horizontal,"负责人2姓名",Qt::DisplayRole);
    partModel.setHeaderData(5,Qt::Horizontal,"负责人2电话",Qt::DisplayRole);
    partModel.select();
    ui->partTableView->setModel(&partModel);
    ui->partTableView->verticalHeader()->setVisible(false);
    ui->partTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    alarmModel.setTable("alarmImfor");
    alarmModel.setHeaderData(0,Qt::Horizontal,"防区编号",Qt::DisplayRole);
    alarmModel.setHeaderData(1,Qt::Horizontal,"防区名称",Qt::DisplayRole);
    alarmModel.setHeaderData(2,Qt::Horizontal,"负责人1姓名",Qt::DisplayRole);
    alarmModel.setHeaderData(3,Qt::Horizontal,"负责人1电话",Qt::DisplayRole);
    alarmModel.setHeaderData(4,Qt::Horizontal,"负责热2姓名",Qt::DisplayRole);
    alarmModel.setHeaderData(5,Qt::Horizontal,"负责人2电话",Qt::DisplayRole);
    alarmModel.setHeaderData(6,Qt::Horizontal,"防区类型",Qt::DisplayRole);
    alarmModel.setHeaderData(8,Qt::Horizontal,"防区类型",Qt::DisplayRole);
    alarmModel.select();
    ui->alarmTableView->setModel(&alarmModel);
    ui->alarmTableView->verticalHeader()->setVisible(false);
    ui->alarmTableView->setColumnHidden(6,true);
    ui->alarmTableView->setColumnHidden(7,true);
    ui->alarmTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

DataEntryDialog::~DataEntryDialog()
{
    delete ui;
}

void DataEntryDialog::on_partDelePushButton_clicked()
{
    QModelIndexList indexList = ui->partTableView->selectionModel()->selectedIndexes();
    QModelIndex index;
    foreach (index, indexList) {
        partModel.removeRow(index.row());
    }
    partModel.select();
}

void DataEntryDialog::on_partAddPushButton_clicked()
{
    EntryForm *form = new EntryForm(this,&partModel);
    form->show();
}

void DataEntryDialog::on_partSaveButton_clicked()
{
    int row = ui->partTableView->currentIndex().row();
    EntryForm *form = new EntryForm(this,&partModel,row);
    form->show();
}

void DataEntryDialog::on_alarmAddpushButton_clicked()
{
    EntryForm *form = new EntryForm(this,&alarmModel,-1,FORM_TYPE::ALARM_FORM);
    form->show();
}



void DataEntryDialog::on_alarmEditPushButton_clicked()
{
    int row = ui->alarmTableView->currentIndex().row();
    if(row == -1)
    {
        return;
    }
    EntryForm *form = new EntryForm(this,&alarmModel,row,FORM_TYPE::ALARM_FORM);
    form->show();
}

void DataEntryDialog::on_alarmDelPushButton_clicked()
{
    QModelIndexList indexList = ui->alarmTableView->selectionModel()->selectedRows();
    QModelIndex index;
    foreach (index, indexList) {
        alarmModel.removeRow(index.row());
    }
    alarmModel.select();
}
