#ifndef IMFORPARSE_H
#define IMFORPARSE_H
#include <QString>
#include <QList>
#include "node.h"
#include "z1801p.h"

struct AlarmImf
{
   AlarmImf()
   {
       alarmNum = 0;
       alarmStatu = AlarmStatu::NoStaru;
       alarmName = QString();
   }
   AlarmImf(int num,AlarmStatu s,QString name)
   {
       alarmNum = num;
       alarmStatu = s;
       alarmName = name;
   }
   int alarmNum;//防区编号
   AlarmStatu alarmStatu;//防区状态
   QString alarmName;//防区名称
};

struct PartImf
{
    int num;//分区编号1~8
    QString partName;//分区名称
    PartStatus partStatu;//分区状态
    QList<AlarmImf> alarmList;//防区列表
};
struct AlarmDeviceImf
{
    QString deviceName;//主机名称
    Node::Statu deviceStatu;//主机状态
    QList<PartImf> partList;//分区列表
};

class ImforParse
{
public:
    ImforParse();
    Node *parse(const AlarmDeviceImf &deviceImfor,bool isLogin);
    Node::Statu StatuByAlarmNum(int num);
    Node::Statu NodeStatuFromPartStatus(PartStatus s);
    Node::Statu NodeStatuFromAlarmStatus(AlarmStatu s);
private:
    void addChild(Node *parent, Node *child);
    QMap<int,Node::Statu> alarmStatuMap;
};

#endif // IMFORPARSE_H
