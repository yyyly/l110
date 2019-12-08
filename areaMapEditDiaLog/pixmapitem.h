#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include "z1801p.h"

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>
#include <QTimer>


struct AreaOption
{
    AreaOption(int n,AlarmType t,QPointF p)
        :num(n),
         type(t),
         pos(p){}
    AreaOption(){}
    int num;
    AlarmType type;
    QPointF pos;
};
class PixmapItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    PixmapItem(AreaOption o, QGraphicsItem *parent = Q_NULLPTR,QObject * p = 0);
    void setAlarmName(QString str);
    int getNum(){return option.num;}
    AreaOption &getAreaOption();
    void setMovealble(bool e){isMoveable = e;}
    friend bool operator ==(const PixmapItem &,const PixmapItem &);
protected:
    QRectF boundingRect() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
    void changePixmap();
private:
    QPointF startPos;
    QString alarmName;
    AreaOption option;
    bool state_on = true;
    QPixmap *nor_pixmap;
    QPixmap *hove_pixmap;
    QTimer *timer;
    bool isMoveable;

};

inline bool operator == (const PixmapItem &item1,const PixmapItem &item2)
{

 return item1.option.num == item2.option.num;
}

#endif // PIXMAPITEM_H
