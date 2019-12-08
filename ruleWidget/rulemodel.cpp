#include "rulemodel.h"
#include<QColor>
#include<QRgb>
#include<QByteArray>
#include<QString>
#include<QUrl>
#include<QHeaderView>
#include<QTreeWidgetItem>
#include<QWidget>
RuleModel::RuleModel(QObject *parent, QSqlDatabase db)
    :QSqlTableModel(parent,db)
{

}

Qt::ItemFlags RuleModel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant RuleModel::data(const QModelIndex &idx, int role) const
{
    switch (role) {
    case Qt::BackgroundColorRole:
    {
        if(idx.column() == 1)
        {
            QByteArray d = "ff" + idx.data().toByteArray().remove(0,1);
            QRgb c = d.toUInt(nullptr,16);
            return QColor(c);
            break;
        }
    }
    case Qt::TextAlignmentRole:
    {
        return Qt::AlignCenter;
    }
        break;
    default:
        return this->QSqlTableModel::data(idx,role);
        break;
    }
}
