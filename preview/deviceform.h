#ifndef DEVICEFORM_H
#define DEVICEFORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QStringList>
#include "deviceimforsqltablemodel.h"

namespace Ui {
class DeviceForm;
}

class DeviceForm : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceForm(QWidget *parent = nullptr,DeviceImforSqlTableModel *m = nullptr,int n = -1);
    ~DeviceForm();

private slots:
    void on_submitButton_clicked();

private:
    Ui::DeviceForm *ui;
    int num;
    DeviceImforSqlTableModel *model;
    QDataWidgetMapper *mapper;
    static QStringList modeStringList;
    static QStringList typeStringList;
};

#endif // DEVICEFORM_H
