#ifndef MAPSCENCE_H
#define MAPSCENCE_H
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include "pixmapitem.h"

class MapScence : public QGraphicsScene
{
    Q_OBJECT
public:
    MapScence(QObject *parent = 0);
    MapScence(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);
protected:
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
signals:
    dropFinished(int i);
private:
    QGraphicsItem *currentItem;
public slots:
    void removeCurrentItem();
};

#endif // MAPSCENCE_H
