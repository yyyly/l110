#ifndef Z1801_H
#define Z1801_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<z1801p.h>
#include<QDateTime>
#include<QList>
#include<QTimer>

#include"devicemodel/imforparse.h"

#define l110Port int
#define Byte quint8

struct AlarmMessing{
    QDateTime time;
    AlarmType type;
    int zoneId;
    int aLarmID;
};

struct DeviceStatu
{
    quint8 zoneNum;//使用的分区数
    ZoneStatu zoneStatus[8];//分区状态
};

struct AlarmNode
{
    int alarmID;
    AlarmStatu alarmStatu;
};

struct ZoneImfors{
  int zoneID;
  QList<int> alarmlist;
};

struct Assist//辅助结构体
{
    int sign;//标记
    PartType type;
    int part;
};

struct AlarmProperty
{
    int alarmNum;
    int alarmType;
    int part;
    bool bypassEnable;
    bool isControlByHomeKey;
    ModuleType module;
    static const int propertyNum = 6;
};

struct deviceDelayTime
{
    int alarmDelayTime;
    int defenceDelayTime;
    int eleRelayTime;
    int bellTime;
};

struct autoDefenceImfor
{
    int defenceHour;
    int defenceMintue;
    int outDefenceHour;
    int outDefenceMintue;
    int weekDay[7];
};

struct KeyOption
{
    QByteArray keyProperty;
    QByteArray keyOfPart;
    bool isValid(){return !keyProperty.isEmpty();}
};

class Z1801 : public QObject
{
    Q_OBJECT
public:
    explicit Z1801(QObject *parent = 0);
    ~Z1801();
    void getDeviceTime();

    void getDeviceStatus();

    void getPartAreaNum(PartType type,int part);
    void getAlarmImfByAddress(int l,int h);

    void defenceDevice(int part,bool defence);

    QByteArray buildCMD(QByteArray str);

    qint64 sendCmd(QByteArray cmd);

    bool isLogin();
    AlarmStatu * getAlarmsStatu();

    QList<int> getZoneAlarms(int n);

    QByteArray intToByte(int number);

    void reSendPartImfor();
signals:
    void deviceLogin();

    void haveAlarmMessing(AlarmMessing messing);

    void partAlarmsChange();

    void getDeviceImfFinished(AlarmStatu *statuArray,int *partArray);

    void alarmImfByAddress(QList<AlarmProperty> imfList);

    void partCoumt(int n);//使用分区数

    void updatePartImfor(const PartImf &imf);

    void barMessing(const QString &messing);

    void delayTime(const deviceDelayTime &t);
    
    void defenceImfor(const autoDefenceImfor &imfor);

    void keyMessing(const KeyOption &option);
private:
    QTcpSocket *tcpId;
    QTcpSocket tcpSocket;
    QTcpServer *server;
    QByteArray *alarmImformation;
    QDateTime deviceTime;
    bool deviceTimeIsFreshed = false;
    void init(l110Port port);
    void checkData(QByteArray *alarmImfor);//处理原始数据
    void  parse(QByteArray *alarmImfor);
    QMap<QString, QVariant> result;
    char readBit(char &c,int pos);
    bool dateIsFull(QByteArray date);
    int n;
    //主机状态
    DeviceStatus status;
    ZoneImfors zoneImfors;
    AlarmStatu alarmStatuArray[192];//主机防区属性
    int alarmPartArray[192];//防区所属分区
    Assist assistArray[13];
    void initAsistArray();
    QTimer timer;

    
private slots:
    void newConnect();
    void getRequest();
public slots:
     void getDeviceImf();

};

#endif // Z1801_H
