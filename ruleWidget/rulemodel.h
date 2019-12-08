#ifndef RULEMODEL_H
#define RULEMODEL_H
#include<QSqlTableModel>

class RuleModel : public QSqlTableModel
{
public:
    RuleModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
protected:
    QVariant data(const QModelIndex &idx,int role)const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // RULEMODEL_H
