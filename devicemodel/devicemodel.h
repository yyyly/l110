#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H
#include<QAbstractItemModel>
#include<QModelIndex>
#include<QVariant>

#include"node.h"

class DeviceModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    DeviceModel(QObject *parent);
    ~DeviceModel();

    void setRootNode(Node *node);

    void resetRootNode(Node *node);

    QModelIndex indexFromAlarm(int alarm);

    QModelIndex indexFromUserData(int part,int alarm);//通过分区和防区找到index

    QList<QModelIndex> indexFromPart(int part);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index,int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:


private:
    Node *nodeFromIndex(const QModelIndex &index) const;
    Node *rootNode;
};

#endif // DEVICEMODEL_H
