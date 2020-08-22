#include "z1801.h"
#include "globaloption.h"
#include<QDataStream>
#include<QDebug>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include<QTimeZone>
#include<QDataStream>
#include<QByteRef>
#include<QJsonParseError>
#include<QJsonDocument>
#include<QElapsedTimer>
#include<QDebug>
#include<synchapi.h>



Z1801::Z1801(QObject *parent)
    : QObject(parent),
      heartTimer(new QTimer(this))
{
    init(55562);
    initAsistArray();
    status = DeviceStatus::DisLogin;
    connect(&timer,SIGNAL(timeout()),this,SLOT(getDeviceImf()));
    connect(heartTimer,&QTimer::timeout,this,&Z1801::deviceOffline);
}

Z1801::~Z1801()
{
    server->close();
}

void Z1801::init(int port)
{
    server = new QTcpServer(this);
    alarmImformation = new QByteArray;
    server->listen(QHostAddress::Any,port);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnect()));
}

void Z1801::newConnect()
{
    tcpId = server->nextPendingConnection();
    //status = DeviceStatus::DisLogin;
    initAsistArray();
    connect(tcpId,SIGNAL(readyRead()),this,SLOT(getRequest()));
}

void Z1801::getRequest()
{
    QByteArray buffer = tcpId->readAll();
    QByteArrayList bufferList = buffer.split('~');
    bufferList.removeAll("");
    foreach (buffer, bufferList) {
        *alarmImformation = buffer.toHex();
        qDebug() << *alarmImformation << endl;
        alarmImformation->replace("7d01","7d");
        alarmImformation->replace("7d02","7e");//转义

        if(!dateIsFull(QByteArray::fromHex(*alarmImformation)))
        {
            return;
        }
        alarmImformation->remove(0,2);//去掉校验码
        checkData(alarmImformation);
    }

}

void Z1801::deviceOffline()
{
    emit deviceOff();
}


bool Z1801::dateIsFull(QByteArray date)
{
    char d = 0;
    foreach(char i,date)
    {
        d += i;
    }
    return d== 0 ? true:false;
}



void Z1801::checkData(QByteArray *alarmImfor)
{
    QByteArray type = alarmImfor->left(2);
    if(type == "01")
    {
        QByteArray subType = alarmImfor->remove(0,2).left(4);
        if(subType == "0183")//终端的心跳包
        {
            QByteArray cmd = buildCMD("8201030101");
            sendCmd(cmd);
            //status = DeviceStatus::Logined;
            result["messingType"] = MessingType::HeartBeat;
            if(status == DeviceStatus::DisLogin)
            {
                status = DeviceStatus::Logined;
                emit deviceLogin();
                timer.start(800);
            }
            if(heartTimer->isActive())
            {
                heartTimer->stop();
                heartTimer->start(120000);
            }
            return;
        }
        parse(alarmImfor);
    }
    else if(type == "10")//设备登录
    {
        initAsistArray();
        QByteArray cmd = buildCMD("9001");
        sendCmd(cmd);//登录，校验成功
        status = DeviceStatus::Logined;
        emit deviceLogin();
        timer.start(800);
        if(heartTimer->isActive())
        {
            heartTimer->stop();
            heartTimer->start(120000);
        }
    }
}

void Z1801::reSendPartImfor()
{
    timer.start(800);
}

void Z1801::defenceDevice(int part, bool defence)
{
    QByteArray array = "82100c02";
    if(defence)
    {
        array = array + "66";
    }
    else {
        array = array + "67";
    }
    QByteArray a[9] = {"00","01","02","03","04","05","06","07","08"};
    if(part >= 0 && part < 9)//在这里有说明，注意part的边界条件，是从0开始的，少了一个等号，调试了半天
    {
        array = array + a[part];
    }
    QByteArray cmd = buildCMD(array);
    sendCmd(cmd);
}

bool Z1801::isLogin()
{
    if(status == DeviceStatus::DisLogin)
    {
        return  false;
    }
    else {
        return true;
    }
}


void Z1801::parse(QByteArray *alarmImfor)
{
    QByteArray type = alarmImfor->remove(0,2).left(2);
    if(type == "9f")
    {
        AlarmMessing messing;
        QByteArray cmd = buildCMD("81109f0101");
        sendCmd(cmd);
        QByteArray timeByte = alarmImfor->remove(0,4).left(8);
        int timeInt[4];
        for(int i = 0;i < 4;i++)
        {
            timeInt[i]=timeByte.left(2).toInt();
            timeByte.remove(0,2);
        }
        int year = QDateTime::currentDateTime().date().year();
        QDate date = QDate(year,timeInt[0],timeInt[1]);
        QTime time = QTime(timeInt[2],timeInt[3]);
        QDateTime alarmTime = QDateTime(date,time);
        messing.time = alarmTime;
        QByteArray a = alarmImfor->remove(0,8).left(4);
        qDebug() << "报警类型" << a << endl;
        int alarmType =  a.toInt(Q_NULLPTR,16);
        messing.type = (AlarmType)alarmType;
        int  deviceTypeValue = alarmImfor->remove(0,4).left(2).toInt(Q_NULLPTR,16);
        messing.zoneId = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
        char s = alarmImfor->at(2) - 48;
        char h = alarmImfor->at(3) - 48;
        char t = alarmImfor->at(4) - 48;
        char g = alarmImfor->at(5) - 48;
        messing.aLarmID = s * 1000 + h * 100 + t * 10 + g;
        emit haveAlarmMessing(messing);
    }
    else if(type == "95")//返回终端时间
    {
        int year = alarmImfor->remove(0,6).left(4).toInt();
        int month = alarmImfor->remove(0,4).left(2).toInt();
        int day = alarmImfor->remove(0,2).left(2).toInt();
        int hour = alarmImfor->remove(0,2).left(2).toInt();
        int minute = alarmImfor->remove(0,2).left(2).toInt();
        int secon = alarmImfor->remove(0,2).left(2).toInt();
        deviceTime = QDateTime(QDate(year,month,day),QTime(hour,minute,secon));

    }
    else if(type == "a0")//返回主机工作状态
    {
        DeviceStatu device;
        QByteArray deviceStatu = QByteArray::fromHex(*alarmImfor);
        quint8 partNum = (quint8)deviceStatu.left(1)[0];
        device.zoneNum = partNum;
        deviceStatu.remove(0,1);
        for(int i = 0;i < 8;i++)
        {
            char statu = deviceStatu.left(1)[0];
            if(readBit(statu,0))
            {
                if(readBit(statu,5))
                {
                    device.zoneStatus[i] = ZoneStatu::DefReady;
                }
                else {
                    device.zoneStatus[i] = ZoneStatu::DefNotReady;
                }
            }
            else {
                if(readBit(statu,5))
                {
                    device.zoneStatus[i] = ZoneStatu::OutDefReady;
                }
                else {
                    device.zoneStatus[i] = ZoneStatu::OutDefNotReady;
                }
            }
            deviceStatu.remove(0,3);
        }
    }
    else if(type == "a2")//分区防区信息
    {
        int messingType = alarmImfor->remove(0,4).left(2).toInt(Q_NULLPTR,16);
        int n = alarmImfor->remove(0,2).left(2).toInt();//分区
        switch (messingType) {
        case PartType::AREA_MESSING://分区包含的防区信息
        {
            QByteArray arrhex =alarmImfor->remove(0,2).left(48);
            QByteArray arr = QByteArray::fromHex(arrhex);
            int i = 0;
            foreach (char byte, arr)
            {
                for(int j = 0;j < 8;j++)
                {
                    if(readBit(byte,j))
                    {
                        int num = 8 * i + j;
                        alarmPartArray[num] = n;
                    }
                }
                i++;
            }
            assistArray[n-1].sign = 1;
            qDebug() << QString("第%1分区防区获取成功").arg(n)<< endl;
            emit barMessing(QString("第%1分区防区获取成功").arg(n));
        }
            break;
        case PartType::DEFENCE_MESSING:
        {
            PartImf partImf;
            partImf.num = n;
            QByteArray arrhex =alarmImfor->remove(0,2).left(48);
            QByteArray arr = QByteArray::fromHex(arrhex);
            int i =0;
            AlarmImf alarmImf;
            foreach (char byte, arr) {
                for(int j = 0;j < 8;j++)
                {
                    int num = 8 * i + j;
                    if(readBit(byte,j))
                    {
                        alarmStatuArray[num] = AlarmStatu::Defence;
                        alarmImf.alarmNum = num + 1;
                        alarmImf.alarmStatu = AlarmStatu::Defence;
                        partImf.alarmList.append(alarmImf);
                    }
                    else {
                        if(!(alarmStatuArray[num] == AlarmStatu::UN_USED))
                        {
                            alarmStatuArray[num] = AlarmStatu::OutDefence;
                        }
                    }
                }
                i++;
            }
            assistArray[9].sign = 1;
            emit updatePartImfor(partImf);
            emit barMessing("获取防区布防信息成功");
            qDebug() << "获取防区布防信息成功"  << endl;
        }
            break;
        case PartType::BYPASS_MESSING:
        {
            PartImf partImf;
            partImf.num = n;
            QByteArray arrhex =alarmImfor->remove(0,2).left(48);
            QByteArray arr = QByteArray::fromHex(arrhex);
            int i =0;
            AlarmImf alarmImf;
            foreach (char byte, arr) {
                for(int j = 0;j < 8;j++)
                {
                    if(readBit(byte,j))
                    {
                        int num = 8 * i + j;
                        alarmStatuArray[num] = AlarmStatu::ByPass;
                        alarmImf.alarmNum = num + 1;
                        alarmImf.alarmStatu = AlarmStatu::ByPass;
                        partImf.alarmList.append(alarmImf);
                    }
                }
                i++;
            }
            assistArray[10].sign = 1;
            emit updatePartImfor(partImf);
            emit barMessing("获取旁路信息成功");
            qDebug() << "获取旁路信息成功"  << endl;
        }
            break;
        case PartType::FAULT_MESSING://故障
        {
            PartImf partImf;
            partImf.num = n;
            QByteArray arrhex =alarmImfor->remove(0,2).left(48);
            QByteArray arr = QByteArray::fromHex(arrhex);
            int i =0;
            AlarmImf alarmImf;
            foreach (char byte, arr) {
                for(int j = 0;j < 8;j++)
                {
                    if(readBit(byte,j))
                    {
                        int num = 8 * i + j;
                        alarmStatuArray[num] = AlarmStatu::FAULT;
                        alarmImf.alarmNum = num + 1;
                        alarmImf.alarmStatu = AlarmStatu::FAULT;
                        partImf.alarmList.append(alarmImf);
                    }
                }
                i++;
            }
            assistArray[11].sign = 1;
            emit updatePartImfor(partImf);
            emit barMessing("获取防区错误信息成功");
            qDebug() << "获取防区错误信息成功"  << endl;
        }
            break;
        case PartType::USED_ALARM_MESSING:
        {
            QByteArray arrhex =alarmImfor->remove(0,2  ).left(48);
            QByteArray arr = QByteArray::fromHex(arrhex);
            int i =0;
            foreach (char byte, arr) {
                for(int j = 0;j < 8;j++)
                {
                    int num = 8 * i + j;
                    if(!readBit(byte,j))
                    {
                        alarmStatuArray[num] = AlarmStatu::UN_USED;
                    }
                }
                i++;
            }
            assistArray[8].sign = 1;
            emit barMessing("获取已使用防区信息成功");
            qDebug() << "获取已使用防区信息成功"<< endl;
        }
            break;
        default:
            break;
        }
    }
    else if(type == "a3") //返回终端存储类容
    {
        int startAddress =  alarmImfor->remove(0,4).left(4).toInt(Q_NULLPTR,16);
        if(startAddress > 0x06fb && startAddress < 0x0b00)//返回防区信息
        {
            QByteArrayList arrayList;
            while(!alarmImfor->isEmpty())
            {
                QByteArray array = alarmImfor->remove(0,8).left(8);
                if(!array.isEmpty())
                {
                    arrayList.append(array);
                }
            }
            QByteArray a;
            QList<AlarmProperty> list;
            AlarmProperty p;
            int alarmStartNum;
            int n = (startAddress -  WRIED_ALARM_L_ADDRESS) / 32;
            alarmStartNum = 1 + 8 * n;
            foreach (a, arrayList) {
                p.alarmNum = alarmStartNum++;
                p.alarmType = a.left(2).toInt(Q_NULLPTR,16);
                p.part = a.remove(0,2).left(1).toInt(Q_NULLPTR,16) +0x01;
                p.bypassEnable = (bool)a.remove(0,1).left(1).toInt(Q_NULLPTR,16);
                p.isControlByHomeKey = (bool)a.remove(0,1).left(1).toInt(Q_NULLPTR,16);
                p.module = (ModuleType)a.remove(0,1).left(1).toInt(Q_NULLPTR,16);
                list.append(p);
            }
            QByteArray reply = buildCMD("8201100101");
            sendCmd(reply);
            emit alarmImfByAddress(list);
        }
        else if(startAddress == 0x04e0)//使用分区数
        {
            int n = alarmImfor->remove(0,8).left(2).toInt(Q_NULLPTR,16);
            emit partCoumt(n + 1);
        }
        else if(startAddress == 0x0520)//报警，布防延时时间等
        {
            deviceDelayTime time;
            time.alarmDelayTime= alarmImfor->remove(0,8).left(2).toInt(Q_NULLPTR,16);
            time.defenceDelayTime = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
            time.eleRelayTime = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
            time.bellTime = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
            emit delayTime(time);
        }
        else if(startAddress > 0x0602 && startAddress < 0x0683)
        {
            autoDefenceImfor imfor;
            imfor.defenceHour = (alarmImfor->at(8) -48) * 10 + (alarmImfor->at(9) -48);
            imfor.defenceMintue = (alarmImfor->at(10) -48) * 10 + (alarmImfor->at(11) -48);
            imfor.outDefenceHour = (alarmImfor->at(12) -48) * 10 + (alarmImfor->at(13) -48);
            imfor.outDefenceMintue = (alarmImfor->at(14) -48) * 10 + (alarmImfor->at(15) -48);
            alarmImfor->remove(0,15);
            for(int i = 0;i < 7;i++)
            {
                imfor.weekDay[i] = alarmImfor->remove(0,1).left(1).toInt(Q_NULLPTR,16);
            }
            emit defenceImfor(imfor);
        }
        else if(startAddress == 0x04e4)//键盘配置
        {
            KeyOption keyOption;
            keyOption.keyProperty = alarmImfor->remove(0,8).left(8);
            keyOption.keyOfPart = alarmImfor->remove(0,16).left(8);
            emit keyMessing(keyOption);
        }
    }
    else if(type == "ab")//返回终端对码操作
    {
        CodeImfor imf;
        imf.handleType = 0;
        imf.deviceType = alarmImfor->remove(0,4).left(2).toInt(Q_NULLPTR,16);
        imf.add = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
        int mt = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
        switch (mt) {
        case 0:
        {
            imf.messing = "无线地址不存在";
        }
            break;
        case 1:
        {
            imf.messing = "主机进入对码状态";
        }
            break;
        case 2:
        {
            imf.messing = "地址已存在无线设备，请先删除";
        }
            break;
        case 3:
        {
            imf.messing = "对码成功";
        }
            break;
        case 4:
        {
            imf.messing = "无线设备已对码到主机";
        }
            break;
        default:
            break;
        }
        emit codeMessing(imf);
    }
    else if(type == "ac")//返回终端删除设备操作
    {
        CodeImfor imf;
        imf.handleType = 1;
        imf.deviceType = alarmImfor->remove(0,4).left(2).toInt(Q_NULLPTR,16);
        imf.add = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
        int mt = alarmImfor->remove(0,2).left(2).toInt(Q_NULLPTR,16);
        switch (mt) {
        case 0:
        {
            imf.messing = "删除成功";
        }
            break;
        case 1:
        {
            imf.messing = "删除失败";
        }
            break;
        default:
            break;
        }
        emit codeMessing(imf);
    }

    else if(type == "b1")//恢复出厂，这里需要开通一下前两个遥控，是报警主机不足的补充
    {

    }

}

void Z1801::getDeviceTime()
{
    QByteArray str = "82100501c3";
    QByteArray cmd = buildCMD(str);
    sendCmd(cmd);
}

void Z1801::getDeviceStatus()
{
    QByteArray str = "82100601f2";
    QByteArray cmd = buildCMD(str);
    sendCmd(cmd);
}

void Z1801::learnCode(int type, int add)
{
    QByteArray typeArray = intToByte(type).mid(2,2);
    QByteArray addArray = intToByte(add).mid(2,2);
    QByteArray str = "82101802" + typeArray + addArray;
    QByteArray cmd = buildCMD(str);
    sendCmd(cmd);
}

void Z1801::quiteLearnCode()
{
    QByteArray str = "82101a0155";
    QByteArray cmd = buildCMD(str);
    sendCmd(cmd);
}

void Z1801::deleteCode(int type, int add)
{
    QByteArray typeArray = intToByte(type).mid(2,2);
    QByteArray addArray = intToByte(add).mid(2,2);
    QByteArray str = "82101902" + typeArray + addArray;
    QByteArray cmd = buildCMD(str);
    sendCmd(cmd);
}


void Z1801::reset()
{
    QByteArray str = "82101f0155";
    QByteArray cmd = buildCMD(str);
    sendCmd(cmd);
}
void Z1801::getPartAreaNum(PartType type, int part)
{
    //char buf[2];
    //sprintf(buf,"%02x",type);
    //QByteArray str1 = "82100d02" + QByteArray(buf,2);
    //sprintf(buf,"%02x",part);
    QByteArray typeArray = intToByte(type).mid(2,2);
    QByteArray partArray = intToByte(part).mid(2,2);
    QByteArray  str = "82100d02" + typeArray + partArray;
    qDebug() << "........xxxxxx......xxxxxx: " << str << endl;
    QByteArray cmd = buildCMD(str);

    //qDebug() << "........xxxxxx......xxxxxx: " << str << ":::" << cmd << endl;
    switch (type) {
    case PartType::USED_ALARM_MESSING:
    {
        emit barMessing("获取已使用的防区");
        qDebug() << "获取已使用的防区：" << endl;
    }
        break;
    case PartType::AREA_MESSING:
    {
        emit barMessing(QString("获取第%1分区使用的防区").arg(part));
        qDebug() << QString("获取第%1分区使用的防区").arg(part) << endl;
    }
        break;
    case PartType::FAULT_MESSING:
    {
        emit barMessing("获取故障防区");
    }
        break;
    case PartType::BYPASS_MESSING:
    {
        emit barMessing("获取旁路防区：");
    }
        break;
    case PartType::DEFENCE_MESSING:
    {
        emit barMessing("获取布防防区");
    }
        break;
    default:
        break;
    }
    sendCmd(cmd);
}

QByteArray Z1801::intToByte(int number)
{
    QByteArray abyte0;
    abyte0.resize(4);
    int n0 = 0x0000000f & number;
    int n1 = (0x000000f0 & number) >> 4;
    int n2 = (0x00000f00 & number) >> 8;
    int n3 = (0x0000f000 & number) >> 12;
    abyte0[3] = n0 < 10 ? (n0 + 48) : (n0 + 87);
    abyte0[2] = n1 < 10 ? (n1 + 48) : (n1 + 87);
    abyte0[1] = n2 < 10 ? (n2 + 48) : (n2 + 87);
    abyte0[0] = n3 < 10 ? (n3 + 48) : (n3 + 87);
    return abyte0;
}

void Z1801::getAlarmImfByAddress(int l, int h)
{
    QByteArray lb = intToByte(l);
    QByteArray hb = intToByte(h);
    QByteArray str = "82100e04" + lb + hb;
    QByteArray cmd = buildCMD(str);
    qDebug() << "........********......*********: " << str << endl;
    sendCmd(cmd);
}

qint64 Z1801::sendCmd(QByteArray cmd)
{
    qint64 size = tcpId->write(cmd);
    return size;
}

QByteArray Z1801::buildCMD(QByteArray str)
{
    //str.replace("7e","7d02");
    //str.replace("7d","7d01");
    QByteArray bcd = QByteArray::fromHex(str);
    char j = 0;
    foreach(char i,bcd)
    {
        j += i;
    }
    char checkSum = QByteArray::fromHex("ff")[0] - j + 1;
    bcd.prepend(checkSum);

    char d = QByteArray::fromHex("7d")[0];
    QByteArray dArr = QByteArray::fromHex("7d01");
    bcd.replace(d,dArr);
    char e = QByteArray::fromHex("7e")[0];
    QByteArray eArr = QByteArray::fromHex("7d02");
    bcd.replace(e,eArr);

    bcd.prepend(QByteArray::fromHex("7e")[0]);
    bcd.append(QByteArray::fromHex("7e")[0]);
    return bcd;
}

char Z1801::readBit(char &c, int pos)
{
    char b_mask = 0x01;
    b_mask = b_mask << pos;
    if((c&b_mask) == b_mask)
        return 1;
    else
        return 0;
}


AlarmStatu* Z1801::getAlarmsStatu()
{
    return alarmStatuArray;
}

void Z1801::initAsistArray()
{
    for(int i =0;i <8;i++)
    {
        assistArray[i].sign = 0;
        assistArray[i].type = PartType::AREA_MESSING;
        assistArray[i].part = i+1;
    }
    assistArray[8].part = 0;
    assistArray[8].type = PartType::USED_ALARM_MESSING;
    assistArray[8].sign = 0;
    assistArray[9].part = 0;
    assistArray[9].type = PartType::DEFENCE_MESSING;
    assistArray[9].sign = 0;
    assistArray[10].part = 0;
    assistArray[10].type = PartType::BYPASS_MESSING;
    assistArray[10].sign = 0;
    assistArray[11].part = 0;
    assistArray[11].type = PartType::FAULT_MESSING;
    assistArray[11].sign = 0;

    assistArray[12].sign = 0;//结束标志

}

void Z1801::getDeviceImf()
{
    int i = 0;
    while(assistArray[i++].sign == 1);
    if(i == 13)
    {
        timer.stop();
        emit(getDeviceImfFinished(alarmStatuArray,alarmPartArray));
        return;
    }
    PartType type = assistArray[--i].type;
    int part = assistArray[i].part;
    getPartAreaNum(type,part);
}


