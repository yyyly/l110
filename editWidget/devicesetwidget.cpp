#include "devicesetwidget.h"
#include "devicesetitemdelegate.h"
#include "ui_devicesetwidget.h"
#include <QDateTime>
#include <QDate>
#include <QTime>


DeviceSetWidget::DeviceSetWidget(QWidget *parent,Z1801 *z) :
    QDialog(parent),
    ui(new Ui::DeviceSetWidget),
    model(new DeviceSetModel(this,z)),
    z1801(z),
    isSetReplay(false),
    bar(new CStatuBar(this)),
    replayNum(0)
{
    ui->setupUi(this);
    ui->busComboBox->hide();
    ui->wireLessComboBox->hide();
    ui->wireRadioButton->setChecked(true);
    ui->tableView->setItemDelegate(new DeviceSetItemDelegate());
    ui->tableView->setModel(model);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setEditTriggers(QAbstractItemView::CurrentChanged);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->p1->setCheckable(true);
    this->setWindowTitle("主机设置");
    int w = (this->width() - bar->width()) / 2;
    int h = this->height() - bar->height() -80;
    bar->setGeometry(w,h,160,40);
    bar->raise();
    bar->hide();
    ui->defenceHourSpinBox->setRange(0,24);
    ui->defenceMintueSpinBox->setRange(0,24);
    ui->outdefenceHourSpinBox->setRange(0,59);
    ui->outdefenceMintueSpinBox->setRange(0,59);
    connect(ui->pollButton,SIGNAL(clicked()),this,SLOT(on_pollButton_clicked()));
    connect(ui->setButton,SIGNAL(clicked()),this,SLOT(on_setButton_clicked()));
    connect(ui->busRadioButton,SIGNAL(clicked(bool)),this,SLOT(on_busRadioButton_clicked(bool)));
    connect(ui->wireRadioButton,SIGNAL(clicked(bool)),this,SLOT(on_wirelessRadioButton_clicked()));
    connect(z1801,SIGNAL(partCoumt(int)),this,SLOT(partNumUpdate(int)));
    connect(z1801,SIGNAL(alarmImfByAddress(QList<AlarmProperty>)),
            this,SLOT(newAlarmPropertys(QList<AlarmProperty>)));
    connect(z1801,SIGNAL(delayTime(const deviceDelayTime &)),this,SLOT(delayTimeUpdate(const deviceDelayTime &)));
    connect(z1801,SIGNAL(defenceImfor(const autoDefenceImfor &)),this,SLOT(defenceTimeUpdate(const autoDefenceImfor &)));
    connect(z1801,SIGNAL(keyMessing(const KeyOption &)),this,SLOT(updateKeyMessing(const KeyOption &)));
}

DeviceSetWidget::~DeviceSetWidget()
{
    delete ui;
}

void DeviceSetWidget::newAlarmPropertys(QList<AlarmProperty> propertyList)
{
    if(isSetReplay)
    {
        bar->showMessage("设置成功",1000);
        replayNum++;
        if(replayNum == 2)
        {
            isSetReplay = false;
            replayNum = 0;
        }
    }
    else {
        model->setAlarmPropertys(propertyList);
        bar->showMessage("查询成功",1000);
    }

}

int DeviceSetWidget::getStartAddress()
{
    int baseNum = 0;
    int n = 0;
    if(ui->wireRadioButton->isChecked())
    {
        baseNum = 0;
    }
    else if(ui->busRadioButton->isChecked())
    {
        baseNum = 1;
        n = ui->busComboBox->currentIndex();
    }
    else {
        baseNum = 15;
        n = ui->wireLessComboBox->currentIndex();
    }
    int startAddress = WRIED_ALARM_L_ADDRESS + (baseNum + n) * 32;
    return startAddress;
}

void DeviceSetWidget::on_pollButton_clicked()
{
    if(!z1801)
        return;
    int n = getStartAddress();
    if(z1801->isLogin())
    {
        z1801->getAlarmImfByAddress(n,n + 30);
    }
}

void DeviceSetWidget::on_setButton_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    QByteArray len_2;
    QByteArray lAddressBuf;
    QByteArray hAddressBuf;
    QByteArray command = model->serialize();
    lAddressBuf = z1801->intToByte(getStartAddress());
    hAddressBuf = z1801->intToByte(getStartAddress() + 30);
    QByteArray str1 = lAddressBuf + hAddressBuf + command;
    int len = str1.length() / 2;
    len_2 = z1801->intToByte(len).mid(2,2);
    QByteArray str = "821010" + len_2 + str1;
    QByteArray writeData = z1801->buildCMD(str);
    z1801->sendCmd(writeData);
    isSetReplay = true;//
}


void DeviceSetWidget::on_busRadioButton_clicked(bool checked)
{
    if(checked)
    {
        ui->busComboBox->show();
        ui->wireLessComboBox->hide();
    }
}

void DeviceSetWidget::on_wirelessRadioButton_clicked(bool checked)
{
    if(checked)
    {
        ui->wireLessComboBox->show();
        ui->busComboBox->hide();
    }
}

void DeviceSetWidget::on_wireRadioButton_clicked(bool checked)
{
    if(checked)
    {
       ui->wireLessComboBox->hide();
       ui->busComboBox->hide();
    }
}
//开通分区数查询
void DeviceSetWidget::on_partNumInquireButton_clicked()
{
    if(!z1801)
        return;
    int n = 0x04e0;
    if(z1801->isLogin())
    {
        z1801->getAlarmImfByAddress(n,n);//partCoumt(int n)信号返回结果
    }
}

void DeviceSetWidget::partNumUpdate(int c)
{
    int b = c - 16;
    ui->partNumSpinBox->setValue(b);
}

void DeviceSetWidget::on_partNumEditButton_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    int v = ui->partNumSpinBox->value() - 1;
    QByteArray len_2;
    QByteArray AddressBuf = z1801->intToByte(0x04e0);
    QByteArray cmd_2 = z1801->intToByte(v + 16).mid(2,2);
    QByteArray str1 = AddressBuf + AddressBuf + cmd_2;
    int len = str1.length() / 2;
    len_2 = z1801->intToByte(len).mid(2,2);
    QByteArray str = "821010" + len_2 + str1;
    QByteArray writeData = z1801->buildCMD(str);
    z1801->sendCmd(writeData);
}
//查询报警延时，布防延时，警号输出时间，继电器输出时间
void DeviceSetWidget::on_timeInquirePushButton_clicked()
{

    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    int start = 0;
    int end = 0;
    if(z1801->isLogin())
    {
        start = 0x0520;
        end = 0x0524;
        z1801->getAlarmImfByAddress(start,end);
    }
}

void DeviceSetWidget::on_timeEditPushButton_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    QByteArray lAddressBuf = z1801->intToByte(0x0520);
    QByteArray hAddressBuf = z1801->intToByte(0x0524);
    QByteArray a1 = z1801->intToByte(ui->alarmDelaySpinBox->value()).mid(2,2);
    QByteArray a2 = z1801->intToByte(ui->defenceDelaySpinBox->value()).mid(2,2);
    QByteArray a3 = z1801->intToByte(ui->relayLastTimeSpinBox->value()).mid(2,2);
    QByteArray a4 = z1801->intToByte(ui->sperkerLastTimeSpinBox->value()).mid(2,2);
    QByteArray str1 = lAddressBuf + hAddressBuf + a1 + a2 + a3 + a4;
    int len = str1.length() / 2;
    QByteArray len_2 = z1801->intToByte(len).mid(2,2);
    QByteArray str = "821010" + len_2 + str1;
    QByteArray writeData = z1801->buildCMD(str);
    z1801->sendCmd(writeData);
}

void DeviceSetWidget::delayTimeUpdate(const deviceDelayTime &t)
{
    ui->alarmDelaySpinBox->setValue(t.alarmDelayTime);
    ui->defenceDelaySpinBox->setValue(t.defenceDelayTime);
    ui->relayLastTimeSpinBox->setValue(t.eleRelayTime);
    ui->sperkerLastTimeSpinBox->setValue(t.bellTime);
}

void DeviceSetWidget::on_remoterLermButton_clicked()
{

}

void DeviceSetWidget::on_detecterLermButton_clicked()
{

}



void DeviceSetWidget::on_defenceTimeInquirePushButton_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    int baseAdd = 0x0604;
    int i = ui->partNumComboBox->currentIndex();
    int j = ui->partSubGroupCombox->currentIndex();
    int startAdd = baseAdd + 16 * i + 8 * j;
    int endAdd = startAdd + 0x7;
    z1801->getAlarmImfByAddress(startAdd,endAdd);
}

void DeviceSetWidget::on_defenceTimeEditPushTime_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    int baseAdd = 0x0604;
    int i = ui->partNumComboBox->currentIndex();
    int j = ui->partSubGroupCombox->currentIndex();
    int startAdd = baseAdd + 16 * i + 8 * j;
    int endAdd = startAdd + 0x7;
    int defenceH = ui->defenceHourSpinBox->value();
    int defenceM = ui->defenceMintueSpinBox->value();
    int outdefenceH = ui->outdefenceHourSpinBox ->value();
    int outdefenceM = ui->outdefenceMintueSpinBox->value();
    QByteArray startAddArray_4 = z1801->intToByte(startAdd);
    QByteArray endAddArray_4 = z1801->intToByte(endAdd);
    QByteArray defenceHarray_2 = z1801->intToByte(defenceH / 10).mid(3,1) + z1801->intToByte(defenceH % 10).mid(3,1);
    QByteArray defenceMarray_2 = z1801->intToByte(defenceM / 10).mid(3,1) + z1801->intToByte(defenceM % 10).mid(3,1);
    QByteArray outdefenceHarray_2 = z1801->intToByte(outdefenceH / 10).mid(3,1) + z1801->intToByte(outdefenceH % 10).mid(3,1);
    QByteArray outdefenceMarray_2 = z1801->intToByte(outdefenceM / 10).mid(3,1) + z1801->intToByte(outdefenceM % 10).mid(3,1);
    QByteArray weekArray;
    weekArray.resize(8);
    weekArray[0] = ui->p1->isChecked() ? '1' : '0';
    weekArray[1] = ui->p2->isChecked() ? '1' : '0';
    weekArray[2] = ui->p3->isChecked() ? '1' : '0';
    weekArray[3] = ui->p4->isChecked() ? '1' : '0';
    weekArray[4] = ui->p5->isChecked() ? '1' : '0';
    weekArray[5] = ui->p6->isChecked() ? '1' : '0';
    weekArray[6] = ui->p7->isChecked() ? '1' : '0';
    weekArray[7] = '0';
    QByteArray str1 = startAddArray_4 + endAddArray_4 + defenceHarray_2 + defenceMarray_2
            + outdefenceHarray_2 + outdefenceMarray_2 + weekArray;
    int len = str1.length() / 2;
    QByteArray len_2 = z1801->intToByte(len).mid(2,2);
    QByteArray str = "821010" + len_2 + str1;
    QByteArray writeData = z1801->buildCMD(str);
    z1801->sendCmd(writeData);

}

void DeviceSetWidget::defenceTimeUpdate(const autoDefenceImfor &imf)
{
    ui->defenceHourSpinBox->setValue(imf.defenceHour);
    ui->defenceMintueSpinBox->setValue(imf.defenceMintue);
    ui->outdefenceHourSpinBox->setValue(imf.outDefenceHour);
    ui->outdefenceMintueSpinBox->setValue(imf.outDefenceMintue);
    ui->p1->setChecked(imf.weekDay[0] == 1);
    ui->p2->setChecked(imf.weekDay[1] == 1);
    ui->p3->setChecked(imf.weekDay[2] == 1);
    ui->p4->setChecked(imf.weekDay[3] == 1);
    ui->p5->setChecked(imf.weekDay[4] == 1);
    ui->p6->setChecked(imf.weekDay[5] == 1);
    ui->p7->setChecked(imf.weekDay[6] == 1);
}



void DeviceSetWidget::on_setDeviceTimepushButton_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    QDateTime dateTime = QDateTime::currentDateTime();
    QDate d = dateTime.date();
    QTime t = dateTime.time();
    QByteArray a;
    QByteArray year_h = z1801->intToByte((d.year() % 100) / 10).mid(3,1);
    QByteArray year_l = z1801->intToByte((d.year() % 100) % 10).mid(3,1);
    QByteArray mont_h = z1801->intToByte(d.month() / 10).mid(3,1);
    QByteArray mont_l = z1801->intToByte(d.month() % 10).mid(3,1);
    QByteArray day_h = z1801->intToByte(d.day() / 10).mid(3,1);
    QByteArray day_l = z1801->intToByte(d.day() % 10).mid(3,1);
    QByteArray week_h = z1801->intToByte(d.dayOfWeek() / 10).mid(3,1);
    QByteArray week_l = z1801->intToByte(d.dayOfWeek() % 10).mid(3,1);
    QByteArray hour_h = z1801->intToByte(t.hour() / 10).mid(3,1);
    QByteArray hour_l = z1801->intToByte(t.hour() % 10).mid(3,1);
    QByteArray minute_h = z1801->intToByte(t.minute() / 10).mid(3,1);
    QByteArray minute_l = z1801->intToByte(t.minute() % 10).mid(3,1);
    QByteArray second_h = z1801->intToByte(t.second() / 10).mid(3,1);
    QByteArray second_l = z1801->intToByte(t.second() % 10).mid(3,1);
    if(d.dayOfWeek() == 7)
    {
        week_h = z1801->intToByte(0).mid(3,1);
        week_l = z1801->intToByte(0).mid(3,1);
    }
    a.append(year_h).append(year_l).append(mont_h).append(mont_l)
            .append(day_h).append(day_l).append(week_h).append(week_l)
            .append(hour_h).append(hour_l).append(minute_h).append(minute_l)
            .append(second_h).append(second_l);
    //QByteArray aHex = a.toHex();
    QByteArray len_2 = z1801->intToByte(a.length() / 2).mid(2,2);
    QByteArray str = "821012" + len_2 + a;
    QByteArray writeData = z1801->buildCMD(str);
    z1801->sendCmd(writeData);
 }

void DeviceSetWidget::on_keyInquirepushButton_clicked()
{
    if(!z1801)
        return;
    int start = 0;
    int end = 0;
    if(z1801->isLogin())
    {
        start = 0x04e4;
        end = 0x04ef;
        z1801->getAlarmImfByAddress(start,end);
    }
}

void DeviceSetWidget::on_keyEditpushButton_clicked()
{
    if(!z1801)
        return;
    if(!z1801->isLogin())
        return;
    if(keyOption.isValid())
    {
        int keyNumIndex = ui->keyNumComboBox->currentIndex();
        char ckeyPro = z1801->intToByte(ui->keyPropertyComboBox->currentIndex()).mid(3,1)[0];
        char ckeyOfPart = z1801->intToByte(ui->keyofPartComboBox->currentIndex()).mid(3,1)[0];
        keyOption.keyProperty[keyNumIndex] = ckeyPro;
        keyOption.keyOfPart[keyNumIndex] = ckeyOfPart;
        QByteArray lAddressBuf = z1801->intToByte(0x04e4);
        QByteArray hAddressBuf = z1801->intToByte(0x04ef);
        //主机设计是可以接16个键盘，在使用的过程中根据经验，根本用不到这么多键盘，固只考虑了8个键盘的设置，其中“00000000”是另外
        //8个键盘的设置，一直都设置成0了。
        QByteArray str1 = lAddressBuf + hAddressBuf + keyOption.keyProperty + "00000000" + keyOption.keyOfPart;
        int len = str1.length() / 2;
        QByteArray len_2 = z1801->intToByte(len).mid(2,2);
        QByteArray str = "821010" + len_2 + str1;
        QByteArray writeData = z1801->buildCMD(str);
        z1801->sendCmd(writeData);
    }
}

void DeviceSetWidget::updateKeyMessing(const KeyOption &option)
{
    keyOption = option;
    int keyNumIndex = ui->keyNumComboBox->currentIndex();
    char ckeyPro = option.keyProperty[keyNumIndex];
    char ckeyofPart = option.keyOfPart[keyNumIndex];
    int keyproIndex = (int)ckeyPro - 48;
    int keyofPartIndex = (int)ckeyofPart - 48;
    ui->keyPropertyComboBox->setCurrentIndex(keyproIndex);
    ui->keyofPartComboBox->setCurrentIndex(keyofPartIndex);
}
