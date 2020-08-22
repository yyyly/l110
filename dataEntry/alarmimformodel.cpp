#include "alarmimformodel.h"

AlarmImforModel::AlarmImforModel(QObject *parent)
    :QSqlTableModel(parent)
{

}

QVariant AlarmImforModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
    {
        return  QVariant();
    }
    case Qt::TextAlignmentRole:
    {
        return Qt::AlignCenter;
    }

    default:
    {
     return QSqlTableModel::data(index,role);
    }
  }
}
