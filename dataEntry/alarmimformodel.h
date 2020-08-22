#ifndef ALARMIMFORMODEL_H
#define ALARMIMFORMODEL_H
#include<QSqlTableModel>

class AlarmImforModel : public  QSqlTableModel
{
    Q_OBJECT
public:
    AlarmImforModel(QObject *parent = nullptr);
protected:

    QVariant data(const QModelIndex &index,int role) const;

};

#endif // ALARMIMFORMODEL_H
