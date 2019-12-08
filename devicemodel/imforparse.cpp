/*这个类负责生成设备防区树的底层数据结构*/
#include "imforparse.h"
#include "node.h"

#include<QJsonParseError>
#include<QJsonDocument>
#include<QVariantList>
#include<QDebug>
#include<QVariantMap>


ImforParse::ImforParse()
{

}

/*----设备不在线时为设备树提供一个根节点
Node *ImforParse::parse(QString deviceName)
{
    Node *invalidNode = new Node();
    Node *rootNode = new Node(Node::Device,Node::OffLine,deviceName,0)

}*/

Node *ImforParse::parse(const AlarmDeviceImf &deviceImfor, bool isLogin)
{
    Node *invalidNode = new Node();
    Node *rootNode = new Node(Node::Device,Node::OffLine,deviceImfor.deviceName,0);
    addChild(invalidNode,rootNode);
    if(!isLogin)
    {
        return invalidNode;
    }
    PartImf part;
    foreach (part, deviceImfor.partList) {
        Node::Statu partStatu = NodeStatuFromPartStatus(part.partStatu);
        QString partName = part.partName;
        int partNum = part.num;
        Node *partNode = new Node(Node::Partition,partStatu,partName,partNum);
        addChild(rootNode,partNode);
        AlarmImf alarm;
        foreach (alarm, part.alarmList) {
            Node::Statu alarmStatu = NodeStatuFromAlarmStatus(alarm.alarmStatu);
            QString alarmName = alarm.alarmName;
            int alarmNum = alarm.alarmNum;
            Node *alarmNode = new Node(Node::Alarm,alarmStatu,alarmName,alarmNum);
            addChild(partNode,alarmNode);
        }
    }
    return invalidNode;
}


Node::Statu ImforParse::StatuByAlarmNum(int num)
{
    if(alarmStatuMap.contains(num))
    {
        return alarmStatuMap.value(num);
    }
    else
        return Node::NoStaru;
}

Node::Statu ImforParse::NodeStatuFromPartStatus(PartStatus s)
{
    switch (s) {
    case PartStatus::READY:
        return Node::Statu::Enale;

    case PartStatus::NOREADY:
        return  Node::Statu::DisEnale;

    default:
        return Node::Statu::NoStaru;

    }
}

Node::Statu ImforParse::NodeStatuFromAlarmStatus(AlarmStatu s)
{
    switch (s) {
    case AlarmStatu::Defence:
        return Node::Statu::Defence;

    case AlarmStatu::OutDefence:
        return Node::Statu::OutDefence;

    case AlarmStatu::ByPass:
        return Node::Statu::ByPass;

    default:
        return Node::Statu::NoStaru;


}
}


void ImforParse::addChild(Node *parent, Node *child)
{
    parent->childen += child;
    child->parent = parent;
}
