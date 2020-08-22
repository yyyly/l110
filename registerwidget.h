#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QDialog>

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

    QString get_SerialNumber();
    QString hash_Encryption(const QString temp);
    QString format_HASHString(const QString hashtemp);

private slots:

    void on_registerButton_clicked();

    void on_copyButton_clicked();

private:
    Ui::RegisterWidget *ui;
};

#endif // REGISTERWIDGET_H
