#ifndef AREAMAP_H
#define AREAMAP_H

#include <QWidget>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QGraphicsItem>
#include<QBrush>
#include<QPixmap>
#include<QRect>
#include<QWheelEvent>
#include<QPushButton>
#include<QResizeEvent>
#include<QDialog>
#include<QAbstractListModel>
#include<QListWidget>
#include<QLabel>
#include "areaMapEditDiaLog/addareamap.h"
#include "cameronet.h"
#include "screen.h"
#include "cstatubar.h"

class QStatusBar;
class CDockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CDockWidget(QWidget *parent = 0);
    void addTab(const QString &str);
    void deleteTab(int row);
signals:
    void tabChanged(int index);
public slots:

public:
    QPushButton *addButton;
    QPushButton *button;
    QLabel *label;
    QListWidget *tabList;
    QPushButton *spreadButton;
    QLabel *vLabel;
    QPushButton *editButton;
    QPushButton *deleteButton;
private slots:
    void itemChanged(int row);
    void draw();

};


class AreaMap : public QWidget
{
    Q_OBJECT
public:
    explicit AreaMap(QWidget *parent = 0);
private:
    MapScence *scence;
    CustomView *view;
    QPushButton *addArea;
    AddAreaMap *addAreaDialog;
    CDockWidget *dockWidget;
    QList<AreaMapOption> areaMapOptionList;
    Screen *screen;
    //QStatusBar *bar;
    CStatuBar *bar;
    LONG playId;
    QList<int> noBindList;
    int currentPlayNum;//目前正在播放的防区，初始值为-1,表示没有正在播放的防区
signals:

public slots:
    void newAreaMap(AreaMapOption option);
    void areaMapChanged(int n);
    void alarmNumChecked(int num);
    void closeChannal();
    void addArea_Clicked();
    void editArea_Clicked();
    void deleteArea_clicked();
    void updateAreaMap(AreaMapOption *option);
    void CloseScreen();
protected:
    void resizeEvent(QResizeEvent *event);
};



#endif // AREAMAP_H