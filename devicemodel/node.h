#ifndef NODE_H
#define NODE_H
#include<QString>
#include<QList>
class Node
{
public:
    enum Statu{Online,OffLine,Enale,DisEnale,Defence,OutDefence,ByPass,NoStaru};
    enum NodeType{Device,Partition,Alarm,NoType};

    Node();
    Node(NodeType type,Statu statu,const QString &str,int num);
    ~Node();

    bool addChild(Node *node);

    NodeType type;
    Statu statu;
    QString str;//结点名称
    Node *parent;
    int num  = 0;
    QList<Node *> childen;
};

#endif // NODE_H
