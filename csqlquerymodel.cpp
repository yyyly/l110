#include "csqlquerymodel.h"

CSqlQueryModel::CSqlQueryModel(const QMap<QString, Ruler> *rul,QObject *parent)
    :QSqlQueryModel (parent),
      rulers(rul)
{

}

QVariant CSqlQueryModel::data(const QModelIndex &idx, int role) const
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
        return this->QSqlQueryModel::data(idx,role);
        break;
    }
}
