#ifndef DEVICESETMODEL_H
#define DEVICESETMODEL_H

#include<QAbstractTableModel>
#include<QList>
#include<QString>
#include<QStringList>
#include<QVariant>
#include"z1801.h"


class DeviceSetModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DeviceSetModel(QObject *parent = 0,Z1801 *z = nullptr);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index,int role) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role);
    QVariant headerData(int section,Qt::Orientation orientation,int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QByteArray serialize();//序列化
private:
    QList<AlarmProperty> alarms;
    static const QStringList alarmTypeStringList;
    static const QStringList protertys;
    Z1801 *z1801;
public slots:
    void setAlarmPropertys(QList<AlarmProperty> alarmPropertys);
};



#endif // DEVICESETMODEL_H
