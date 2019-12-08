#include "changeaccountdidalog.h"
#include "ui_changeaccountdidalog.h"
#include "QSqlQuery"

ChangeAccountDidalog::ChangeAccountDidalog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeAccountDidalog)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec(QString("SELECT ACCOUNT FROM logImfor"));
    QString account;
    while (query.next()) {
        account = query.value(0).toString();
        ui->comboBox->addItem(account);
    }
}

ChangeAccountDidalog::~ChangeAccountDidalog()
{
    delete ui;
}



QString ChangeAccountDidalog::getCurrentAccount()
{
    return ui->comboBox->currentText();
}

void ChangeAccountDidalog::on_okPushButton_clicked()
{
    QString account = ui->comboBox->currentText();
    QString passWard = ui->lineEdit->text();
    QSqlQuery query;
    query.exec(QString("SELECT ACCOUNT,PASSWORD FROM logImfor "
                       "WHERE (ACCOUNT = '%1' AND PASSWORD = '%2')").arg(account).arg(passWard));
    if(query.next())
    {
        accept();
    }
    else {
        ui->message->setText("密码错误!");
    }
}
