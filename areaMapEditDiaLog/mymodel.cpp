#include "mymodel.h"
#include "globdata.h"
#include <QVector>
#include <QSqlQuery>


MyModel::MyModel(const QList<int> &list,QObject *parent)
    :QAbstractListModel(parent)
{
    this->list = list;
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    return list.count();
}

int MyModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    {
        int i = list.at(index.row());
        return QString("%1防区----").arg(i) + nameMap.value(i);
    }
        break;
    case Qt::EditRole:
    {
        int i = index.row();
        return list.at(i);
    }
        break;
    default:
        return QVariant();
        break;
    }
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row() >= 0 && index.row() < list.count() && role == Qt::EditRole || role == Qt::DisplayRole)
    {
        list.replace(index.row(),value.toInt());
        QVector<int> roles;
        roles.reserve(2);
        roles.append(Qt::EditRole);
        roles.append(Qt::DisplayRole);
        emit dataChanged(index,index,roles);
        return true;
    }
    return false;
}

bool MyModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row);
    list.removeAt(row);
    endRemoveRows();
    return true;
}

bool MyModel::insertRow(int row, const QModelIndex &parent)
{
    if(row < 0 || row > list.count())
        return false;
    beginInsertRows(parent,row,row);
    list.insert(row,-1);
    endInsertRows();
    return true;
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void MyModel::remListVaule(int i)
{
    int n = list.indexOf(i);
    removeRow(n);
    //从这个表格中删除，说明这个防区已经绑定到了防区图上，需要更新alarmImf这个表中的信息。
    //不能在这里写入数据库，如果不保存的话是不能写入的
    //QSqlQuery query;
    //query.exec(QString("UPDATE alarmImfor SET ON_SCENCE = 1 WHERE ALARM_NUM = %1").arg(i));
    bufferList << i;
}

void MyModel::addItem(int value)
{
    int i;
    for(i = 0;i < list.count();i++)
    {
        if(value < list.at(i))
            break;
    }
    insertRow(i);
    setData(this->index(i),value,Qt::EditRole);
}
