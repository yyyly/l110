#include "devicemodel.h"
#include <QJsonDocument>
#include <QByteArray>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
DeviceModel::DeviceModel(QObject *parent)
    :QAbstractItemModel(parent)
{
    rootNode = nullptr;
}

DeviceModel::~DeviceModel()
{
    delete rootNode;
}

void DeviceModel::setRootNode(Node *node)
{
    Node *deviceNode;
    Node *partNode;
    Node *alarmNode;
    if(rootNode == nullptr)
    {
        rootNode = node;
        return;
    }
    foreach (deviceNode, rootNode->childen) {
        foreach (partNode, deviceNode->childen) {
            foreach (alarmNode, partNode->childen) {
                delete alarmNode;
            }
            delete partNode;
        }
        delete deviceNode;
    }
    delete rootNode;
    rootNode = node;
}

void DeviceModel::resetRootNode(Node *node)
{
    beginResetModel();
    setRootNode(node);
    endResetModel();
}

QModelIndex DeviceModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column)
    if(!rootNode || row < 0)
    {
        return QModelIndex();
    }
    Node *parentNode = nodeFromIndex(parent);
    Node *child = parentNode->childen.value(row);
    if(!child)
    {
        return QModelIndex();
    }
    return createIndex(row,column,child);
}

QModelIndex DeviceModel::indexFromUserData(int part, int alarm)
{
    QModelIndex rootIndex;
    QModelIndex partIndex;
    QModelIndex alarmIndex;
    int i = 0;
    rootIndex = index(0,0,QModelIndex());
    Node *deviceNode = rootNode->childen.value(0);
    qDebug() << " node device type:" << data(rootIndex,Qt::UserRole + 2) << endl;
    for(;i < deviceNode->childen.count();i++)
    {
        partIndex = index(i,0,rootIndex);

        qDebug() << " node part type:" << data(partIndex,Qt::UserRole + 2)
                 << "......"<< data(partIndex,Qt::UserRole) << endl;

        if(partIndex.data(Qt::UserRole).toInt() == part)
        {

            break;
        }

    }
    if(i == deviceNode->childen.count())
    {
        if(partIndex.data(Qt::UserRole).toInt() != part)//说明没有找到
        {
            return QModelIndex();
        }
    }
    for(i = 0;i < rowCount(partIndex);i++)
    {
        alarmIndex = index(i,0,partIndex);
        if(alarmIndex.data(Qt::UserRole).toInt() == alarm)
        {
            qDebug() << " node alarm type:" << data(alarmIndex,Qt::UserRole + 2) << endl;
            return alarmIndex;
        }
    }
    return QModelIndex();
}

QModelIndex DeviceModel::indexFromAlarm(int alarm)
{
    QModelIndex deviceIndex;
    QModelIndex partIndex;
    QModelIndex alarmIndex;
    deviceIndex = index(0,0,QModelIndex());
    for(int i = 0;i < rowCount(deviceIndex);i++)
    {
        partIndex = index(i,0,deviceIndex);
        for(int j = 0; j < rowCount(partIndex);j++)
        {
            alarmIndex = index(j,0,partIndex);
            if(alarmIndex.data(Qt::UserRole).toInt() == alarm)
                return alarmIndex;
        }
    }
    return  QModelIndex();
}

QList<QModelIndex> DeviceModel::indexFromPart(int part)
{
    QList<QModelIndex> list;
    if(part != 0)
    {
        QModelIndex rootIndex;
        QModelIndex partIndex;
        QModelIndex alarmIndex;
        Node *deviceNode = rootNode->childen.value(0);
        int i = 0;
        rootIndex = index(0,0,QModelIndex());
        qDebug() << " node device type:" << data(rootIndex,Qt::UserRole + 2) << endl;
        for(;i < deviceNode->childen.count();i++)
        {
            partIndex = index(i,0,rootIndex);
            qDebug() << " node part type:" << data(partIndex,Qt::UserRole + 2) << endl;
            if(partIndex.data(Qt::UserRole).toInt() == part)
                break;
        }
        if(i == deviceNode->childen.count())
        {
            if(partIndex.data(Qt::UserRole).toInt() != part)//说明没有找到
            {
                return  list;
            }
        }
        for(i = 0;i < rowCount(partIndex);i++)
        {
            alarmIndex = index(i,0,partIndex);
            qDebug() << " node alarm type:" << data(alarmIndex,Qt::UserRole + 2) << endl;
            list.append(alarmIndex);
        }
        return  list;
    }
    else {
        QModelIndex deviceIndex;
        QModelIndex partIndex;
        QModelIndex alarmIndex;
        deviceIndex = index(0,0,QModelIndex());
        for(int i = 0;i < rowCount(deviceIndex);i++)
        {
            partIndex = index(i,0,deviceIndex);
            for(int j = 0; j < rowCount(partIndex);j++)
            {
                alarmIndex = index(j,0,partIndex);
                list << alarmIndex;
            }
        }
        return  list;
    }

}

Node *DeviceModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid()){
        return static_cast<Node *>(index.internalPointer());
    }
    else
        return rootNode;
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if(parent.column() > 0)
        return 0;
    Node *parentNode = nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->childen.count();
}

int DeviceModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex DeviceModel::parent(const QModelIndex &child) const
{
    Node *node = nodeFromIndex(child);
    if (!node)
        return QModelIndex();
    Node *parentNode = node->parent;
    if (!parentNode)
        return QModelIndex();
    Node *grandparentNode = parentNode->parent;
    if (!grandparentNode)
        return QModelIndex();

    int row = grandparentNode->childen.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    Node *node = nodeFromIndex(index);
    if(!node)
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
    {
        return node->str;
    }
    case Qt::UserRole:
    {
        return  node->num;
    }

    case (Qt::UserRole + 1):
    {
        return node->statu;
    }

    case (Qt::UserRole + 2):
    {
        return node->type;
    }
    case Qt::TextAlignmentRole:
    {
        return  int(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    default:
        return QVariant();
    }

}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

bool DeviceModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Node *node = nodeFromIndex(parent);
    if(!node)
        return false;
    beginInsertRows(parent,row,row + count - 1);
    for(int i = 0;i < count;i++)
    {
        Node *childNode = new Node();
        node->addChild(childNode);
        childNode->parent = node;
    }
    endInsertRows();
}

bool DeviceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Node *node = nodeFromIndex(index);
    if(!node)
        return false;
    switch (role) {
    case Qt::DisplayRole:
    {
        node->str = value.toString();

    }
        break;
    case Qt::UserRole:
    {
        node->num = value.toInt();
    }
        break;
    case (Qt::UserRole+1):
    {
        node->statu = (Node::Statu)value.toInt();
    }
        break;
    case (Qt::UserRole+2):
    {
        node->type = (Node::NodeType)value.toInt();
    }
        break;
    default:
        break;
    }

 }

Qt::ItemFlags DeviceModel::flags(const QModelIndex &index) const
{
    return  QAbstractItemModel::flags(index);
}

