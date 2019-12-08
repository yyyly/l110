#ifndef ADDANDEDITEDIALOG_H
#define ADDANDEDITEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "deviceimforsqltablemodel.h"

namespace Ui {
class AddAndEditeDialog;
}

class DeviceForm;

class AddAndEditeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAndEditeDialog(QWidget *parent = nullptr);

    ~AddAndEditeDialog();

private slots:
    void on_editPushButton_clicked();

    void on_addPushButton_clicked();

    void on_delePushButton_clicked();

    void on_editChannalPushButton_clicked();

    void on_quitPushButton_clicked();

public slots:
    void updateChannalTableView();
private:
    Ui::AddAndEditeDialog *ui;
    DeviceImforSqlTableModel *model;
    QSqlTableModel *channalModel;
    DeviceForm *deviceForm;
};

#endif // ADDANDEDITEDIALOG_H
