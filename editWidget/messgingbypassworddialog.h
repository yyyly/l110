#ifndef MESSGINGBYPASSWORDDIALOG_H
#define MESSGINGBYPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class MessgingByPassWordDialog;
}

class MessgingByPassWordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessgingByPassWordDialog(QWidget *parent = nullptr);
    ~MessgingByPassWordDialog();

private slots:
    void on_okPushButton_clicked();

    void on_quickPushButton_clicked();

private:
    Ui::MessgingByPassWordDialog *ui;
};

#endif // MESSGINGBYPASSWORDDIALOG_H
