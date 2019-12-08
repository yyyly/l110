#include "csqltablemode.h"
#include<QColor>
CSqlTableMode::CSqlTableMode(const QMap<QString, Ruler> *rul, QObject *parent, QSqlDatabase db)
    :QSqlTableModel(parent,db)
{
    this->rulers = rul;
}

QVariant CSqlTableMode::data(const QModelIndex &idx, int role) const
{
    switch (role) {
    case Qt::TextAlignmentRole:
    {
        return Qt::AlignCenter;
    }
        break;
    case Qt::BackgroundColorRole:
    {
        QModelIndex index = this->index(idx.row(),4);
        QString type = index.data().toString();
        QRgb rgb = rulers->value(type).groundColor;
        return QColor(rgb);
    }
    default:
        return this->QSqlTableModel::data(idx,role);
        break;
    }
}


