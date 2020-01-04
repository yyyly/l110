#ifndef DATAENTRYDIALOG_H
#define DATAENTRYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "entryform.h"

namespace Ui {
class DataEntryDialog;
}

class DataEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataEntryDialog(QWidget *parent = nullptr);
    ~DataEntryDialog();

private slots:


    void on_partDelePushButton_clicked();

    void on_partAddPushButton_clicked();

    void on_partSaveButton_clicked();

    void on_alarmAddpushButton_clicked();

    void on_alarmEditPushButton_clicked();

    void on_alarmDelPushButton_clicked();

    void on_pushButton_clicked();
signals:
    void deleNumList(const QList<int> list);

private:
    Ui::DataEntryDialog *ui;
    QSqlTableModel deviceModel;
    QDataWidgetMapper mapper;
    QSqlTableModel partModel;
    QSqlTableModel alarmModel;
};

#endif // DATAENTRYDIALOG_H
