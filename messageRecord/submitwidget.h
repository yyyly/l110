#ifndef SUBMITWIDGET_H
#define SUBMITWIDGET_H

#include <QWidget>
#include"csqlquerymodel.h"

namespace Ui {
class SubmitWidget;
}

class SubmitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubmitWidget(QWidget *parent = 0);
    void setModel(CSqlQueryModel *model){tableMode = model;}
    ~SubmitWidget();

private slots:
    void on_inquiryPushButton_clicked();
signals:
    void queryResult(int n);
    void filterChanged(const QString filter);
private:
    Ui::SubmitWidget *ui;
    CSqlQueryModel *tableMode;
    QString statrTimeString;
    QString endTimeString;
    QString alarmTypeString;
    int selectNum;
};

#endif // SUBMITWIDGET_H
