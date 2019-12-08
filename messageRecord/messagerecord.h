#ifndef MESSAGERECORD_H
#define MESSAGERECORD_H

#include <QWidget>
#include <QSqlQueryModel>

#include"submitwidget.h"
#include"messageview.h"
#include"csqlquerymodel.h"

class Player;
class BottomStatu;

class MessageRecord : public QWidget
{
    Q_OBJECT
public:
    explicit MessageRecord(QWidget *parent = 0);

signals:
protected:
    void showEvent(QShowEvent *event);

public slots:
    void itemButtonClicked(QModelIndex index);
    void update();
private:
    SubmitWidget *submit;
    MessageView *messageView;
    CSqlQueryModel *model;
    BottomStatu *bottomWidget;
    QMap<QString,Ruler> rulers;
    Player *player;
};

#endif // MESSAGERECORD_H
