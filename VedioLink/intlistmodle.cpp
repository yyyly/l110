#include "intlistmodle.h"
#include <QVariant>


IntListModle::IntListModle(const QList<int> &intList, const QMap<int,QString> &nameMap, const QString &str, QObject *parent)
      :QAbstractTableModel (parent)
{
    names = nameMap;
    numLists = intList;
    userStr = str;
}

int IntListModle::rowCount(const QModelIndex &parent) const
{
    return numLists.count();
}

int IntListModle::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant IntListModle::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    {
        int i = numLists.at(index.row());
        QString name = names.value(i);
        QString displayStr;
        if(!name.isEmpty())
        {
            displayStr =userStr + QString("%1").arg(i) + "(" + name + ")";
        }
        else {
            displayStr = userStr + QString("%1").arg(i);
        }

        return displayStr;
    }

    case Qt::EditRole:
    {
        int i = index.row();
        return numLists.at(i);
    }
    /*case Qt::CheckStateRole:
    {
        if (index.column() == 0)
                {
                    if (rowCheckStateMap.contains(index.row()))
                    return rowCheckStateMap[index.row()] == Qt::Checked ? Qt::Checked
                            : Qt::Unchecked;
                    else
                        return Qt::Unchecked;
                }
    }*/

    default:
        return QVariant();
        break;
    }
}

QVariant IntListModle::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
       {
       case Qt::TextAlignmentRole:
           return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
       case Qt::DisplayRole:
       {
           if (orientation == Qt::Horizontal)
           {
               return userStr + QString("列表");
           }
       }
        break;
       default:
           return QVariant();
       }

       return QVariant();

}

Qt::ItemFlags IntListModle::flags(const QModelIndex &index) const
{
    if (!index.isValid())
           return QAbstractItemModel::flags(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    //flags |= Qt::ItemIsUserCheckable;
    return flags;
}

bool IntListModle::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
           return false;
       if (role == Qt::CheckStateRole && index.column() == 0)
       {
           if (value == Qt::Checked) //
           {
               rowCheckStateMap[index.row()] = Qt::Checked;
           }
           else
           {
               rowCheckStateMap[index.row()] = Qt::Unchecked;
           }
       }
       return true;
}
