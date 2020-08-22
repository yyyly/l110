#include "messgingbypassworddialog.h"
#include "ui_messgingbypassworddialog.h"
#include <QSqlQuery>
#include <QMessageBox>

MessgingByPassWordDialog::MessgingByPassWordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessgingByPassWordDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("密码验证");
    ui->lineEdit->setPlaceholderText("请输入admin账户的密码");
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

MessgingByPassWordDialog::~MessgingByPassWordDialog()
{
    delete ui;
}

void MessgingByPassWordDialog::on_okPushButton_clicked()
{
    QSqlQuery query;
    QString sql = QString("SELECT PASSWORD FROM logImfor WHERE ACCOUNT = \"%1\"").arg("admin");
    query.exec(sql);
    QString passWord;
    if(query.next())
    {
        passWord = query.value(0).toString();
    }
    if(passWord == ui->lineEdit->text())
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this,"警告","密码错误！");
    }
}

void MessgingByPassWordDialog::on_quickPushButton_clicked()
{
    reject();
    close();
}
