#include "mapscence.h"

#include "mylistview.h"
#include <QGraphicsPixmapItem>
#include <QMimeData>
#include <QDebug>
#include <QSqlQuery>
#include <QGraphicsView>
#include "globdata.h"
#include "pixmapitem.h"


MapScence::MapScence(QObject *parent)
    :QGraphicsScene(parent)
{

}


MapScence::MapScence(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    :QGraphicsScene(parent)
{
    setSceneRect(x,y,width,height);

}

void MapScence::mousePressEvent(QMouseEvent *event)
{

}

void MapScence::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
}

void MapScence::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void MapScence::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    MyListView *source = qobject_cast<MyListView *>(event->source());
    if(source)
    {
        int i = event->mimeData()->text().toInt();//防区号
        PixmapItem *item;
        //AlarmType type = typeMap.value(i);//直接在数据库中查询
        AlarmType type;
        QSqlQuery query;
        query.exec(QString("SELECT ALARM_TYPE FROM alarmImfor WHERE ALARM_NUM = %1").arg(i));
        if(query.next())
        {
            type = (AlarmType)query.value(0).toInt();
        }
        AreaOption option(i,type,event->scenePos());
        item = new PixmapItem(option);
        item->setAlarmName(QString("%1防区").arg(i));
        this->addItem(item);
        QRectF rect = this->sceneRect();
        emit dropFinished(i);
    }
    else
    {
        QWidget *source1 = qobject_cast<QWidget *>(event->source());
        if(source1)
        {
            QGraphicsItem *item = focusItem();
            /*this->removeItem(item);
            //?????????
            int i = event->mimeData()->text().toInt();
            AlarmType type;
            QSqlQuery query;
            query.exec(QString("SELECT ALARM_TYPE FROM alarmImfor WHERE ALARM_NUM = %1").arg(i));
            if(query.next())
            {
                type = (AlarmType)query.value(0).toInt();
            }
            PixmapItem *item1;
            AreaOption option(i,type,event->scenePos());
            item1 = new PixmapItem(option);
            item1->setAlarmName(QString("%1防区").arg(i));*/
            int x = event->scenePos().x();
            int y = event->scenePos().y();
            item->setPos(event->scenePos());
            PixmapItem *pixmapItem = dynamic_cast<PixmapItem *>(item);
            pixmapItem->getAreaOption().pos.rx() = x;
            pixmapItem->getAreaOption().pos.ry() = y;
            pixmapItem->setOpacity(1);

            //this->addItem(item1);
        }
    }

}

void MapScence::removeCurrentItem()
{
    this->removeItem(this->focusItem());
}


