#ifndef MANAGEACCOUNTDIALOG_H
#define MANAGEACCOUNTDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QItemDelegate>

class manageAccountModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit manageAccountModel(QObject *parent = nullptr);
    ~manageAccountModel();
    QVariant data(const QModelIndex &idx,int role)const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

namespace Ui {
class ManageAccountDialog;
}

class ManageAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageAccountDialog(QWidget *parent = nullptr);
    ~ManageAccountDialog();

private slots:
    void on_addPushButton_clicked();

    void on_deletePushButton_clicked();

    void on_editPushButton_clicked();

    void on_closePushButton_clicked();

private:
    Ui::ManageAccountDialog *ui;
    manageAccountModel model;
    QPoint mousePos;
    bool mouseIsPress;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MANAGEACCOUNTDIALOG_H
