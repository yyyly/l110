#ifndef COMBOXDATAMODEL_H
#define COMBOXDATAMODEL_H

#include<QAbstractListModel>
#include<QList>

class ComboxDataModel : public QAbstractListModel
{
public:
    ComboxDataModel(QObject *parent = nullptr,int mode = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index,int role) const;
    void initDatesource();
private:
    int dataMode;
    QList<int> dataSource;
};

#endif // COMBOXDATAMODEL_H
