#ifndef DEVICEIMFORSQLTABLEMODEL_H
#define DEVICEIMFORSQLTABLEMODEL_H

#include<QSqlTableModel>
#include<QStringList>


class DeviceImforSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    DeviceImforSqlTableModel(QObject *parent);
    QVariant data(const QModelIndex &idx,int role)const;
private:
    static QStringList modeStringList;
    static QStringList typeStringList;
};

#endif // DEVICEIMFORSQLTABLEMODEL_H
