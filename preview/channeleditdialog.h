#ifndef CHANNELEDITDIALOG_H
#define CHANNELEDITDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class ChannelEditDialog;
}

class ChannelEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChannelEditDialog(QWidget *parent = nullptr,int s = 0,int n = 0,QSqlTableModel *m = nullptr);
    ~ChannelEditDialog();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ChannelEditDialog *ui;
    int num;
    int serial;
    QSqlTableModel *model;
};

#endif // CHANNELEDITDIALOG_H
