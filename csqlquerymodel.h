#ifndef CSQLQUERYMODEL_H
#define CSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include "ruleWidget/rulewidget.h"


class CSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit CSqlQueryModel(const QMap<QString,Ruler> *rul = nullptr,QObject *parent = nullptr);
protected:
    QVariant data(const QModelIndex &idx, int role) const;
private:
    const QMap<QString,Ruler> *rulers;
};

#endif // CSQLQUERYMODEL_H
