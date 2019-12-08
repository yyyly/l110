#include "devicesetmodel.h"

const QStringList DeviceSetModel::alarmTypeStringList = {"防区无用","出/入口延时防区","周边防区","内部防区",
                                                      "日/夜防区","24小时无声防区","24小时有声防区","内部防区(延时)",
                                                      "火警防区","气感防区","医疗防区","门铃防区","防盗防区"};

const QStringList DeviceSetModel::protertys = {"防区编号","防区类型","分区","允许旁路","受在家布防控制","模块类型"};

DeviceSetModel::DeviceSetModel(QObject *parent,Z1801 *z)
    :QAbstractTableModel (parent),
      z1801(z)
{

}

int DeviceSetModel::rowCount(const QModelIndex & /*parent*/) const
{
    return alarms.count();
}

int DeviceSetModel::columnCount(const QModelIndex &/*parent*/) const
{
    return AlarmProperty::propertyNum;
}

QVariant DeviceSetModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role ==Qt::TextAlignmentRole)
    {
         return  int(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    else if(role == Qt::DisplayRole)
    {
        int row = index.row();
                int column = index.column();
                AlarmProperty property = alarms.value(row);
                switch (column) {
                case 0:
                {
                    return  property.alarmNum;
                }
                case 1:
                {
                    int alarmType = property.alarmType;
                    return QString(alarmTypeStringList[alarmType]);
                }
                case 2:
                {
                    int p = property.part;
                    return  QString("分区%1").arg(p);
                }
                case 3:
                {
                    QString str = (property.bypassEnable) ? "是" : "否";
                    return str;
                }
                case 4:
                {
                    QString str = property.isControlByHomeKey ? "是" : "否";
                    return  str;
                }
                case 5:
                {
                    ModuleType m = property.module;
                    QString str = m == ModuleType::Z851A ? "Z851A" : "Z861A";
                    return str;
                }

             }
    }
    return QVariant();
}

QVariant DeviceSetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        switch (role) {
        case Qt::DisplayRole:
        {
            return protertys[section];
        }
        default:
        {
            return QAbstractTableModel::headerData(section,orientation,role);
        }
        }
    }
    else {
        return QAbstractTableModel::headerData(section,orientation,role);
    }
}

void DeviceSetModel::setAlarmPropertys(QList<AlarmProperty> alarmPropertys)
{
    beginResetModel();
    alarms = alarmPropertys;
    endResetModel();
}

Qt::ItemFlags DeviceSetModel::flags(const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        return QAbstractItemModel::flags(index);
    }
    else
    {
        return  QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }

}

/*0:防区编号 1:防区类型 2: 防区所属分区 3： 是否可旁路 4 ：是否受在家布防　５：防区模块类型  */

bool DeviceSetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role = Qt::EditRole)
    {
        int row = index.row();
        int column = index.column();
        switch (column) {
        case 1:
        {
            alarms[row].alarmType = value.toInt();
        }
            break;
        case 2:
        {
            alarms[row].part = value.toInt();
        }
            break;
        case 3:
        {
           alarms[row].bypassEnable = value.toBool();
        }
            break;
        case 4:
        {
            alarms[row].isControlByHomeKey = value.toBool();
        }
            break;
        case 5:
        {
            alarms[row].module = (ModuleType) value.toInt();
        }
            break;
        }
        emit dataChanged(index,index);
        return  true;
    }
    return false;
}

QByteArray DeviceSetModel::serialize()
{
    AlarmProperty p;
    QByteArray typeArr_2;
    QByteArray partArr_1;
    QByteArray passArr_1;
    QByteArray conrroHomeArr_1;
    QByteArray muduArr_1;
    QByteArray str;
    foreach (p, alarms) {
        int alarmType = p.alarmType;//两位
        int part = p.part -1; //本身模块留下的问题
        int isPass = (int)p.bypassEnable;
        int isControByHome = (int)p.isControlByHomeKey;
        int moudle = (int)p.module;
        muduArr_1 = z1801->intToByte(moudle).mid(3,1);
        conrroHomeArr_1 = z1801->intToByte(isControByHome).mid(3,1);
        passArr_1 = z1801->intToByte(isPass).mid(3,1);
        partArr_1 = z1801->intToByte(part).mid(3,1);
        typeArr_2 = z1801->intToByte(alarmType).mid(2,2);
        QByteArray str1 =typeArr_2 + partArr_1 + passArr_1
                + conrroHomeArr_1 + muduArr_1 + "00";
        str += str1;
    }
    return str;
}
