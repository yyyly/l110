#ifndef HANDLEDIALOG_H
#define HANDLEDIALOG_H

#include <QDialog>

namespace Ui {
class HandleDialog;
}

class HandleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HandleDialog(QWidget *parent = nullptr);
    ~HandleDialog();
    void setData(QString p1 = QString(),QString ph1 = QString(),QString p2= QString(),QString ph2= QString());

private slots:
    void on_pushButton_clicked();

private:
    Ui::HandleDialog *ui;
    QString people1;
    QString phone1;
    QString people2;
    QString phone2;
};

#endif // HANDLEDIALOG_H
