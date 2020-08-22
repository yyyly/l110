#include "accountform.h"
#include "ui_accountform.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

AccountForm::AccountForm(QWidget *parent, QSqlTableModel *m, int row) :
    QDialog(parent),
    ui(new Ui::AccountForm),
    mode(m),
    mapper(new QDataWidgetMapper(this)),
    row(row)
{
    ui->setupUi(this);
    setWindowTitle("信息编辑");
    mode->select();
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(mode);
    mapper->addMapping(ui->accountLineEdit,0);
    mapper->addMapping(ui->passwordLineEdit,1);
    mapper->addMapping(ui->nameLineEdit,2);
    mapper->addMapping(ui->checkBox,3);
    if(row == -1)//表示新添加记录
    {
        ui->prePushButton->hide();
        ui->proPushButton->hide();
        int row = mode->rowCount();
        mode->insertRow(row);
        mapper->setCurrentIndex(row);
    }
    else {
        mapper->setCurrentIndex(row);
        if(ui->accountLineEdit->text() == "admin")
        {
            ui->accountLineEdit->setEnabled(false);
        }
    }
    connect(ui->prePushButton,&QPushButton::clicked,mapper,&QDataWidgetMapper::toPrevious);
    connect(ui->proPushButton,&QPushButton::clicked,mapper,&QDataWidgetMapper::toNext);
}

AccountForm::~AccountForm()
{
    delete ui;
    mode->select();
}

void AccountForm::on_submitPushButton_clicked()
{

    if(row == -1)
    {
        if(!mapper->submit())
        {
            qDebug() << mode->lastError().driverText();
        }
        mode->select();
        int row = mode->rowCount();
        mode->insertRow(row);
        mapper->setCurrentIndex(row);
    }
    else
    {
        int r= mapper->currentIndex();
        mapper->submit();
        mode->select();
        mapper->setCurrentIndex(r +1);
    }
}

void AccountForm::on_accountLineEdit_editingFinished()
{
    QSqlQuery query;
    QString account = ui->accountLineEdit->text();
    QString sql = QString("SELECT ACCOUNT FROM logImfor WHERE ACCOUNT = \"%1\"").arg(account);
    query.exec(sql);
    if(query.next())
    {
        ui->explainLabel->setText("账户已存在");
        ui->submitPushButton->setEnabled(false);
    }
    else
    {
        ui->explainLabel->setText("");
        ui->submitPushButton->setEnabled(true);
    }
}
