#include <QDrag>
#include <QApplication>
#include <QWidget>
#include <QMimeData>
#include "globdata.h"
#include "pixmapitem.h"

PixmapItem::PixmapItem(AreaOption o, QGraphicsItem *parent, QObject *p)
      :QObject(p),
      QGraphicsPixmapItem(parent),
      option(o),
      isMoveable(true)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setShapeMode(BoundingRectShape);
    AlarmType type = o.type;
    switch (type) {
    case AlarmType::FIRE:
    {
        setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
    }
        break;
    case AlarmType::STONE_ALARM:
    {
        setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
    }
    break;
    default:
        break;
    }
    setPos(o.pos);
}

QRectF PixmapItem::boundingRect() const
{
    return QRectF(0,0,32,52);
}

void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isMoveable)
    return;
    if(event->button() == Qt::LeftButton)
    {
        setFocus();
        startPos = event->scenePos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void PixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //setPixmap(QPixmap(":/images/tantou_hover.png"));
    //update();
    setToolTip(nameMap.value(getNum()));
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isMoveable)
    return;
    this->setOpacity(0);
    if(event->buttons() & Qt::LeftButton)
    {

        int distance = (event->scenePos() - startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())
        {
            //setPixmap(QPixmap(":/areaMapEditDiaLog/images/touming.png"));
            QDrag *drag = new QDrag(event->widget());
            QMimeData *mimeData = new QMimeData;
            mimeData->setText(QString("%1").arg(getNum()));
            drag->setMimeData(mimeData);
            switch (option.type) {
            case AlarmType::FIRE:
            {
                drag->setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
            }
                break;
            case AlarmType::STONE_ALARM:
            {
                drag->setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
            }
            break;
            default:
                break;
            }
            drag->exec();
        }
        setCursor(Qt::ClosedHandCursor);
    }
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect rect = option->rect;
    QRect textRect = rect.adjusted(0,35,0,0);
    QGraphicsPixmapItem::paint(painter,option,widget);
    painter->save();
    painter->setPen(Qt::blue);
    painter->drawText(textRect,Qt::AlignHCenter,alarmName);
    painter->restore();
}



void PixmapItem::setAlarmName(QString str)
{
    alarmName = str;
}

void PixmapItem::changePixmap()
{
    if(state_on)
    {
        setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
        state_on = false;
    }
    else
    {
        setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
        state_on = true;
    }
}

AreaOption &PixmapItem::getAreaOption()
{
   return option;
}

