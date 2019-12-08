#ifndef CSQLTABLEMODE_H
#define CSQLTABLEMODE_H

#include <QObject>
#include <QSqlTableModel>
#include "ruleWidget/rulewidget.h"
#include "customdata.h"

class CSqlTableMode : public QSqlTableModel
{
public:
    CSqlTableMode(const QMap<QString,Ruler> *rul = nullptr,QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
protected:
    QVariant data(const QModelIndex &idx, int role) const;


private:
    const QMap<QString,Ruler> *rulers;

};
#endif // CSQLTABLEMODE_H
