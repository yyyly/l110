#ifndef BOTTOMSTATU_H
#define BOTTOMSTATU_H

#include <QWidget>
#include <QSqlQueryModel>
#include"globdata.h"

namespace Ui {
class BottomStatu;
}

class BottomStatu : public QWidget
{
    Q_OBJECT

public:
    explicit BottomStatu(QWidget *parent = nullptr);
    void setModle(QSqlQueryModel *m){model = m;}

     ~BottomStatu();
public slots:
    void setTotalRecordCount(int total);
    void setFilter(const QString filter);
private slots:
    void on_firstPushButton_clicked();

    void on_prePushButton_clicked();

    void on_nextPushButton_clicked();
signals:
    void pageChanged();

private:
    Ui::BottomStatu *ui;
    int totalRecordCount;//总记录数
    int pageCount;
    int currentPos;//查询的起始位置
    //每页记录数在全局变量里面
    QSqlQueryModel *model;
    QString filter;
    void upDateStatu();
    void query(int startPos);
};

#endif // BOTTOMSTATU_H
