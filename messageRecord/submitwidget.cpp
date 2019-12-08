#include "submitwidget.h"
#include "ui_submitwidget.h"
#include "globdata.h"
#include<QDateTime>
#include <QSqlQueryModel>
#include <QSqlQuery>


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
}

