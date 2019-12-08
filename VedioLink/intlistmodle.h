#ifndef INTLISTMODLE_H
#define INTLISTMODLE_H

#include <QAbstractTableModel>
#include <QMap>
#include <QList>

class IntListModle : public QAbstractTableModel
{
    Q_OBJECT
public:
    IntListModle(const QList<int> &intList, const QMap<int,QString> &nameMap, const QString &str, QObject *parent = Q_NULLPTR);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
     bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QList<int> numLists;
    QMap<int,QString> names;
     QMap<int, Qt::CheckState> rowCheckStateMap;
     QString userStr;
};

#endif // INTLISTMODLE_H
