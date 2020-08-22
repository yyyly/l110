#include <QDrag>
#include <QApplication>
#include <QWidget>
#include <QMimeData>
#include <QStyleOptionGraphicsItem>
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
    setFlag(ItemIgnoresTransformations,true);
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
    {
        setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
    }
        break;
    }
    setPos(o.pos);

}

QRectF PixmapItem::boundingRect() const
{
    return QRectF(-16,-34,30,52);
}

QPainterPath PixmapItem::shape() const
{
    QPainterPath path;
    path.addRect(-16,-34,30,52);
    return  path;
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
            {
                drag->setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
            }
                break;
            }
            drag->exec();
        }
        setCursor(Qt::ClosedHandCursor);
    }
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRect pixRext = QRect(-16,-34,32,34);
    QRect textRect = QRect(-16,2,32,16);
    painter->drawPixmap(pixRext,this->pixmap());
    painter->setPen(Qt::blue);
    painter->setBrush(QBrush(Qt::blue));
    painter->drawRect(textRect);

    painter->setPen(Qt::white);
    painter->drawText(textRect,Qt::AlignCenter,alarmName);
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

