#include "bombscreen.h"
#include "ui_bombscreen.h"
#include "cameronet.h"
#include "globdata.h"
#include "screen.h"
#include <QSqlQuery>

BombScreen::BombScreen(AlarmImforOnBombScreen imfor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BombScreen)
{
    ui->setupUi(this);
    ui->screenBar->changeScreenNum(1);
    ui->screenBar->setBottonBarVisibal(false);
    alarm_num = imfor.alarmNum;
    ui->alarmNumLable->setText(QString("%1").arg(alarm_num));
    ui->alarmNameLable->setText(imfor.alarmName);
    ui->charge_1_name->setText(imfor.charge1Name);
    ui->charge_1_phone->setText(imfor.charge1Phone);
    ui->charge_2_name->setText(imfor.charge2Nmae);
    ui->charge_2_phone->setText(imfor.chage2Phone);

    //根具防区号查询绑定的视频通道
    QSqlQuery query;
    query.exec(QString("SELECT serial,channal_num FROM vedioLink WHERE ALARM_NUM = %1").arg(alarm_num));
    CameroNet *cameroNet = CameroNet::getInstance();
    while (query.next()) {
        int serial = query.value(0).toInt();
        int channal = query.value(1).toInt();
        //根具serial查询出播放视频需要的信息
        CameraDeviceImf imf = deviceImf[serial];
       ui->screenBar->setNextSelectScreen();
       Screen *s = ui->screenBar->getScreen();
       cameroNet->realPlay(&imf,channal,*s);
    }
}

BombScreen::~BombScreen()
{
    delete ui;
}
