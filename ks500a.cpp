#include "ks500a.h"
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include "log4qt/logger.h"

Ks500a* Ks500a::ks500a = nullptr;

QMap<int,QString> desMap = {
    {0x00,"主机紧急报警"},
    {0x01,"主机防盗报警"},
    {0x02,"主机防盗报警"},
    {0x03,"出入防区报警"},
    {0x04,"主机防火报警"},
    {0x05,"主机被破坏"},
    {0x06,"主机交流电断电"},
    {0x09,"主机上电报告"},
    {0x10,"主机布防"},
    {0x11,"主机撤防"},
    {0x00c,"主机交流电恢复"},
    {0x00d,"主机电池恢复正常"},
    {0x100,"个人紧急报警"},
    {0x101,"防盗报警"},
    {0x102,"防盗报警2"},
    {0x103,"箱门被打开"},
    {0x104,"防火报警"},
    {0x105,"防拆报警"},
    {0x106,"交流电断电"},
    {0x107,"探测器电池低电"},
    {0x108,"探测器通讯故障"},
    {0x109,"电力线断电报警"},
    {0x10a,"漏电报警"},
    {0x10b,"过流报警"},
    {0x10c,"交流电恢复正常"},
    {0x10d,"探测器电池恢复正常"}};


Ks500a::Ks500a(QObject *parent)
    : QObject(parent),
      serial(new QSerialPort(this)),
      mTimer(new QTimer(this))
{
    statu = DeviceStatus::DisLogin;
    connect(mTimer,&QTimer::timeout,this,&Ks500a::haveLongTimeNoData);
    connect(serial, &QSerialPort::readyRead,this,&Ks500a::dataDisplay);
    connect(serial,&QSerialPort::errorOccurred,this,&Ks500a::handleError);
    isReciviData = false;
}

void Ks500a::haveLongTimeNoData()
{
    AlarmMessing am;
    am.time = QDateTime::currentDateTime();
    am.typeDes = "设备离线";
    am.aLarmID = 0;
    mTimer->stop();
    emit messing(am);

}


void Ks500a::openSeria(const QString com)
{
    if(!com.isEmpty())
    {
        serial->setPortName(com);

        serial->close();
        if(serial->open(QIODevice::ReadWrite)){
            serial->clear();
            serial->setBaudRate(QSerialPort::Baud1200,QSerialPort::AllDirections);//设置波特率和读写方向
            serial->setDataBits(QSerialPort::Data8);      //数据位为8位
            serial->setParity(QSerialPort::NoParity); //无校验位
            serial->setStopBits(QSerialPort::OneStop); //一位停止位
            serial->setFlowControl(QSerialPort::NoFlowControl);//无流控制
            serial->setRequestToSend(true);
            emit linkSucced("连接成功");
        }
        timer= new QTimer(this);
        connect(timer,&QTimer::timeout,this,&Ks500a::timeOut);

        timer->start(1000);

    }

}

void Ks500a::handleError(QSerialPort::SerialPortError err)
{
    if(err == QSerialPort::ResourceError){
        emit linkSucced("串口故障");
        timer->stop();
        serial->clearError();
    }
}

void Ks500a::timeOut()
{
    serial->waitForReadyRead(50);
}

void Ks500a::dataDisplay()
{
    AlarmMessing am;
    QByteArray array = serial->readAll();
    Log4Qt::Logger::logger("KS500A")->debug("RECIVE DATA :" + QString( array.toHex()));
    int data = array.toHex().left(2).toInt(nullptr,16);

    if(data == 0xca && !isReciviData)
    {
        QByteArray reply = "A3";
        isReciviData = true;
        if(serial->write(QByteArray::fromHex(reply)) >= 1)
        {
            qDebug() << "replay CA succed" << endl;
            return;
        }
    }
    else if(data == 0xe3 && !isReciviData){
        QByteArray reply = "EE";
        if(serial->write(QByteArray::fromHex(reply)) >=1)
        {
            qDebug() << "replay E3 succed" << endl;
        }
        if(statu == DeviceStatus::DisLogin)
        {
            statu = DeviceStatus::Logined;
            am.time = QDateTime::currentDateTime();
            am.typeDes = "设备上线";
            am.aLarmID = 0;
            emit messing(am);
        }
        mTimer->stop();
        mTimer->start(120000);
        return;
    }
    else {
        if(buffer.length() < 7)
        {
            buffer.append(array);
            return;
        }
        else
        {
            buffer.append(array);
            isReciviData = false;
            QByteArray reply = "EE";
            if(serial->write(QByteArray::fromHex(reply)) >=1)
            {
                qDebug() << "replay EE succed" << endl;
            }
            QByteArray alarmImfor = buffer.toHex();
            buffer.clear();
            //qDebug() << alarmImfor << endl;
            int alarmNum = alarmImfor.left(3).toInt(Q_NULLPTR,16);
            int alarmType = alarmImfor.mid(3,3).toInt(Q_NULLPTR,16);
            QString alarmDes = desMap.value(alarmType);
            qDebug() << alarmNum << alarmType << alarmDes << endl;
            if(!alarmDes.isEmpty() && alarmNum != 227)
            {
                am.time = QDateTime::currentDateTime();
                am.typeDes = alarmDes;
                am.aLarmID = alarmNum;
                emit messing(am);
            }
            if(statu == DeviceStatus::DisLogin)
            {
                statu = DeviceStatus::Logined;
                mTimer->start(120000);
                am.time = QDateTime::currentDateTime();
                am.typeDes = "设备上线";
                am.aLarmID = 0;
                emit messing(am);
            }


        }


    }
}

void Ks500a::closeSeria()
{
    timer->stop();
    serial->close();
    emit linkSucced("串口关闭");
    //ui->textEdit->append("串口关闭");
}

