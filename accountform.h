#ifndef ACCOUNTFORM_H
#define ACCOUNTFORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class AccountForm;
}

class AccountForm : public QDialog
{
    Q_OBJECT

public:
    explicit AccountForm(QWidget *parent = nullptr,QSqlTableModel *m = nullptr,int row = -1);
    ~AccountForm();

private slots:
    void on_submitPushButton_clicked();

    void on_accountLineEdit_editingFinished();

private:
    Ui::AccountForm *ui;
    QSqlTableModel *mode;
    QDataWidgetMapper *mapper;
    int row;
};

#endif // ACCOUNTFORM_H
