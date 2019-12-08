#include "logwidget.h"
#include "ui_logwidget.h"
#include <QSqlQuery>
#include <QSettings>
#include <QMouseEvent>

logWidget::logWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setButtonStyle(ui->closeButton,":/image/toolbar_close.png",4);
    setButtonStyle(ui->toMinButton,":/image/toolbar_min.png",4);
    ui->widget->installEventFilter(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_logButton_clicked()));
}

logWidget::~logWidget()
{
    delete ui;

}

void logWidget::setButtonStyle(QPushButton *button, QString imgsrc, int CutSec)
{
    int img_w=QPixmap(imgsrc).width();
    int img_h=QPixmap(imgsrc).height();
    int PicWidth = img_w/CutSec;
    button->setFixedSize(PicWidth,img_h);
    button->setStyleSheet(QString("QPushButton{border-width: 41px; background-image: url(%1) 0 0 0 %2 repeat repeat;border-width: 0px; border-radius: 0px;}")
    .append("QPushButton::hover{border-image: url(%1) 0 0 0 %3 repeat repeat;}")
    .append("QPushButton::pressed{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
    .append("QPushButton::checked{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
    .append("QPushButton::disabled{border-image: url(%1) 0 0 0 %5 repeat repeat;}")
    .arg(imgsrc).arg(0).arg(PicWidth*1).arg(PicWidth*2).arg(PicWidth*3));
}

bool logWidget::on_logButton_clicked()
{
    QString account = ui->accountlineEdit->text();
    QString password = ui->passwardlineEdit->text();
    QSqlQuery query;
    query.exec(QString("SELECT * FROM logImfor WHERE (ACCOUNT = '%1' AND PASSWORD = '%2')")
               .arg(account).arg(password));
    if(query.next())//说明查找到有记录
    {
        QSettings settings("settings.ini",QSettings::IniFormat);
        if(ui->checkBox_2->isChecked())//保存账户和密码
        {
            settings.beginGroup("ACCOUNT_PASSWARD");
            settings.setValue("IS_SAVED",1);
            settings.setValue("ACCOUNT",ui->accountlineEdit->text());
            settings.setValue("PASSWARD",ui->passwardlineEdit->text());
            settings.endGroup();
        }
        else {
            settings.beginGroup("ACCOUNT_PASSWARD");
            settings.setValue("IS_SAVED",0);
            settings.setValue("ACCOUNT","");
            settings.setValue("PASSWARD","");
            settings.endGroup();
        }
        if(ui->checkBox_1->isChecked())
        {
            settings.beginGroup("ACCOUNT_PASSWARD");
            settings.setValue("IS_AUTO_LOG",1);
            settings.endGroup();
        }
        else {
            settings.beginGroup("ACCOUNT_PASSWARD");
            settings.setValue("IS_AUTO_LOG",0);
            settings.endGroup();
        }
        accept();
        return  true;
    }
    else {
       ui->bar->setText("账户或密码不正确!");
       return false;
    }

}

void logWidget::on_closeButton_clicked()
{
    reject();
}

void logWidget::update(bool isSaved, bool isAutoLog, QString account, QString passward)
{
    if(isSaved)
    {
        ui->checkBox_2->setCheckState(Qt::Checked);
        ui->accountlineEdit->setText(account);
        ui->passwardlineEdit->setText(passward);
    }
    if(isAutoLog)
    {
        ui->checkBox_1->setCheckState(Qt::Checked);
        timer->start(2000);
    }

}

bool logWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget)
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
        }
        return false;
    }
    return false;
}

void logWidget::on_toMinButton_clicked()
{
    this->showMinimized();
}
