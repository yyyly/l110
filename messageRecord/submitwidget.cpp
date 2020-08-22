#include "submitwidget.h"
#include "ui_submitwidget.h"
#include "globdata.h"
#include<QDateTime>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QRegExp>
#include <QFileDialog>
#include <QMessageBox>
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcell.h"
#include <QDateTime>

SubmitWidget::SubmitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubmitWidget)
{
    ui->setupUi(this);
    QDateTime dt = QDateTime::currentDateTime();
    ui->startDateTime->setDateTime(dt);
    ui->startDateTime->setCalendarPopup(true);
    ui->endDateTime->setDateTime(dt);
    ui->endDateTime->setCalendarPopup(true);
    ui->pushButton->setObjectName("cButton");
    ui->inquiryPushButton->setObjectName("cButton");
}

SubmitWidget::~SubmitWidget()
{
    delete ui;
}

void SubmitWidget::on_inquiryPushButton_clicked()
{
    QString filter;
    endTimeString = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    if(ui->timeComboBox->currentText() == "一天内")
    {
     statrTimeString = QDateTime::currentDateTime().addDays(-1).toString("yyyy-MM-dd HH:mm:ss");
    }
    else if(ui->timeComboBox->currentText() == "两天内")
    {
     statrTimeString = QDateTime::currentDateTime().addDays(-2).toString("yyyy-MM-dd HH:mm:ss");
    }
    else if(ui->timeComboBox->currentText() == "三天内")
    {
     statrTimeString = QDateTime::currentDateTime().addDays(-3).toString("yyyy-MM-dd HH:mm:ss");
    }
    else if(ui->timeComboBox->currentText() == "自定义")
    {
     statrTimeString = ui->startDateTime->dateTime().toString("yyyy-MM-dd HH:mm:ss");
     endTimeString = ui->endDateTime->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    }

    if(ui->alarmTypeComboBox->currentText() == "全部")
    {
        filter = QString("REPORT_TIME > '%1' and REPORT_TIME < '%2'").arg(statrTimeString).arg(endTimeString);
    }
    else {
        alarmTypeString = ui->alarmTypeComboBox->currentText();
        filter = QString("REPORT_TIME > '%1' and REPORT_TIME < '%2' and ZONE_TYPE = '%3'")
                .arg(statrTimeString).arg(endTimeString).arg(alarmTypeString);
    }
    QString alarmNumString = ui->alarmNumlineEdit->text();
    //检测输入是否合法

    if(!alarmNumString.isEmpty())
    {
        int alarmNum = alarmNumString.toInt();

        filter = filter + QString("AND ZONE_NUM = %1").arg(alarmNum);
    }
    emit filterChanged(filter);

    QSqlQuery query;
    query.exec(QString("SELECT COUNT() from alarm WHERE %1").arg(filter));
    if(query.next())
    {
        int n = query.value(0).toInt();
        emit queryResult(n);
    }
    QString q = QString("SELECT * FROM alarm  WHERE %1 ORDER BY REPORT_TIME DESC LIMIT 0,%2").arg(filter).arg(page_max_count_record);
    tableMode->setQuery(q);
    while (tableMode->canFetchMore())
        tableMode->fetchMore();
}


void SubmitWidget::on_pushButton_clicked()
{
    QString fileT = QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") + "报警信息";
    QString fileName = QFileDialog::getSaveFileName(this,"另存为",QString("D:/Documents/%1").arg(fileT),"*.xlsx");
    if(!fileName.isEmpty())
    {
        int cn = tableMode->columnCount();
        int rn = tableMode->rowCount();
        QXlsx::Document xlsx;
        xlsx.setColumnWidth(1,16,18);
        xlsx.setColumnWidth(6,25);
        xlsx.setColumnWidth(7,25);
        QXlsx::Format format;
        format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format.setFontBold(true);
        format.setFontSize(14);
        for(int k = 1; k < cn -1;k ++)
        {
            xlsx.write(1,k,tableMode->headerData(k-1,Qt::Horizontal).toString(),format);
        }
        format.setFontBold(false);
        format.setFontSize(12);
        for(int i = 2;i <= rn + 2;i++)
        {
            for(int j = 1;j < cn - 1;j++)
            {
                xlsx.write(i,j,tableMode->index(i-2,j-1).data().toString(),format);
            }
        }
        if(xlsx.saveAs(fileName))
        {
            QMessageBox::information(this,"提示",QString("保存%1记录").arg(rn));
        }
    }

}


