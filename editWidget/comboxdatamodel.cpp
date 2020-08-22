#include "comboxdatamodel.h"

ComboxDataModel::ComboxDataModel(QObject *parent,int mode)
    :QAbstractListModel(parent),
      dataMode(mode)
{
    initDatesource();
}

void ComboxDataModel::initDatesource()
{
    dataSource.clear();
    if(dataMode == 0)//1-32
    {
        for(int i = 1;i < 33;i++)
        {
            dataSource.append(i);
        }
    }
    else//(129-192)
    {
        for(int i = 129; i < 193; i++)
        {
            dataSource.append(i);
        }
    }
}

int ComboxDataModel::rowCount(const QModelIndex &parent) const
{
    return dataSource.length();
}

QVariant ComboxDataModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        int row = index.row();
        return dataSource.at(row);
    }
    else
    {
        return QVariant();
    }
}
