#include "deviceimforsqltablemodel.h"
#include<QModelIndex>

enum {
    SERIAL,
    MODE,
    NAME,
    IP,
    PORT,
    ACCOUNT,
    PASSWARD,
    TYPE,
    CHANNAL_AMOUNT,
    LINE_STATE
};

QStringList DeviceImforSqlTableModel::modeStringList = {"海康","大华"};
QStringList DeviceImforSqlTableModel::typeStringList = {"NVR","IPC"};

DeviceImforSqlTableModel::DeviceImforSqlTableModel(QObject *parent)
    :QSqlTableModel (parent)
{

}

QVariant DeviceImforSqlTableModel::data(const QModelIndex &idx, int role) const
{
    if(role ==Qt::DisplayRole)
    {
        if(idx.column() == MODE)
        {
            int c = idx.data(Qt::EditRole).toInt();
            if(c == -1)
                return QVariant();
            return modeStringList.at(c);
        }
        if(idx.column() == TYPE)
        {
            int c= idx.data(Qt::EditRole).toInt();
            if(c == -1)
                return QVariant();
            return  typeStringList.at(c);
        }
    }
        return this->QSqlTableModel::data(idx,role);
}
