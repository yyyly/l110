#include "bottomstatu.h"
#include "ui_bottomstatu.h"
#include "globdata.h"

BottomStatu::BottomStatu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BottomStatu)
{
    ui->setupUi(this);
    this->hide();
    currentPos = 0;
}

BottomStatu::~BottomStatu()
{
    delete ui;
}

void BottomStatu::setTotalRecordCount(int total)
{
    int x = page_max_count_record;
    totalRecordCount = total;
    int nPage = (total % x == 0) ? (total / x) : (total / x + 1);
    pageCount = nPage;
    upDateStatu();
    this->show();
}

void BottomStatu::upDateStatu()
{
    int currentPage = currentPos / page_max_count_record + 1;
    QString str = QString("%1/%2页，共%3条记录，每页最多%4条")
            .arg(currentPage).arg(pageCount).arg(totalRecordCount).arg(page_max_count_record);
    ui->describleLabel->setText(str);
    if(currentPos == 0)
    {
        ui->prePushButton->setEnabled(false);
    }
    else {
        ui->prePushButton->setEnabled(true);
    }
    if((currentPos + page_max_count_record) > totalRecordCount)
    {
        ui->nextPushButton->setEnabled(false);
    }
    else {
        ui->nextPushButton->setEnabled(true);
    }
}

void BottomStatu::setFilter(const QString f)
{
    filter = f;
}

void BottomStatu::query(int startPos)
{
    if(filter.isEmpty())
        return;
    if(startPos < 0 || startPos > totalRecordCount)
        return;
    if(model == nullptr)
        return;
    QString query = QString("SELECT * FROM alarm  WHERE %1 ORDER BY REPORT_TIME DESC LIMIT %2,%3")
            .arg(filter).arg(startPos).arg(page_max_count_record);
    model->setQuery(query);
    currentPos = startPos;

}



void BottomStatu::on_firstPushButton_clicked()
{
    query(0);
    upDateStatu();
    emit pageChanged();
}

void BottomStatu::on_prePushButton_clicked()
{
    query(currentPos - page_max_count_record);
    upDateStatu();
    emit pageChanged();
}



void BottomStatu::on_nextPushButton_clicked()
{
    query(currentPos + page_max_count_record);
    upDateStatu();
    emit pageChanged();
}
