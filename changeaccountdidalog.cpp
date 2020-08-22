#include "changeaccountdidalog.h"
#include "ui_changeaccountdidalog.h"
#include "QSqlQuery"
#include <QMouseEvent>

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
    ui->widget->installEventFilter(this);
    ui->closePushButton->setButtonStyle(":/image/toolbar_close.png",4);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

ChangeAccountDidalog::~ChangeAccountDidalog()
{
    delete ui;
}

bool ChangeAccountDidalog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget && !this->isMaximized())
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        switch(mouseEvent->type())
        {
        case QEvent::MouseButtonPress:
            {
                if(mouseEvent->button() == Qt::LeftButton)
                {
                    mouseIsPress = true;
                    mousePos = mouseEvent->pos();
                }
            }
                break;
        case QEvent::MouseMove:
            {
                move(mouseEvent->pos() - mousePos  + pos());
            }
                break;
        case QEvent::MouseButtonRelease:
            {
                mouseIsPress = false;
            }
                break;
        default:
            break;
        }
    }
    return false;
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

void ChangeAccountDidalog::on_closePushButton_clicked()
{
    this->close();
}

void ChangeAccountDidalog::on_cancelPushButton_clicked()
{
    this->close();
}
