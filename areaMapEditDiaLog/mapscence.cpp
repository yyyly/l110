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
    :QGraphicsScene(parent),
      bgImage(nullptr)
{
    isMainDisplay = false;
    active = false;
}


MapScence::MapScence(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    :QGraphicsScene(parent)
{
    setSceneRect(x,y,width,height);

}

MapScence::~MapScence()
{
    if(bgImage)
    {
        delete bgImage;
    }
}
void MapScence::setBackImage(const QPixmap *image)
{
    if(bgImage)
    {
        delete bgImage;
        bgImage = nullptr;
    }
    if(image != nullptr)
    {
        bgImage = new QPixmap(*image);
    }

}

void MapScence::mousePressEvent(QMouseEvent *event)
{

}

void MapScence::drawBackground(QPainter *painter, const QRectF &rect)
{
   if(bgImage)
   {
       painter->drawPixmap(0,0,*bgImage);
       setActive(true);
   }
   else if(isMainDisplay)
   {
       QRectF rect = QRectF(0,0,400,300);
       QFont font = painter->font();
       font.setPointSize(16);
       painter->setFont(font);
       QRectF cRect = rect.adjusted(20,18,rect.width(),36);
       painter->drawText(cRect,"欢迎使用接警客户端");
       font.setPointSize(14);
       painter->setFont(font);
       cRect = rect.adjusted(20,56,rect.width(),68);
       painter->drawText(cRect,"为了更好的使用本软件，您需要录入必要信息：");
       font.setPointSize(12);
       font.setFamily("仿宋");
       painter->setFont(font);
       cRect = rect.adjusted(20,88,rect.width(),100);
       painter->drawText(cRect,"①在【信息录入】中录入相应防区信息");
       cRect = rect.adjusted(20,115,rect.width(),127);
       painter->drawText(cRect,"②点击右侧添加防区图按键添加防区图，并拖动防区到防区图上保存");
       cRect = rect.adjusted(20,142,rect.width(),154);
       painter->drawText(cRect,"③在【视频预览】界面添加视频信息");
       cRect = rect.adjusted(20,169,rect.width(),181);
       painter->drawText(cRect,"④在【联动配置】中关联防区和视频通道，报警后可自动弹屏并保存录像");
       cRect = rect.adjusted(20,196,rect.width(),208);
       painter->drawText(cRect,"⑤在【接警规则】中自定义警情显示颜色，声音播报等");
   }
   else
   {
      QGraphicsScene::drawBackground(painter,rect);
   }
}

void MapScence::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
}

void MapScence::setIsMainDisplay(bool is)
{
    isMainDisplay = is;
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
        else
        {
            type = AlarmType::ALARM_TYPE_ALL;
        }
        AreaOption option(i,type,event->scenePos());
        item = new PixmapItem(option);
        item->setAlarmName(QString("%1防区").arg(i));
        this->addItem(item);
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


