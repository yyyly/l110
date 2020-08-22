#ifndef MAPSCENCE_H
#define MAPSCENCE_H
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include "pixmapitem.h"

struct AreaMapOption
{
public:
    AreaMapOption(){}
    QString areaMapName;
    QString backIamgePath;
    QList<AreaOption> areaList;
    QPixmap backImage;
    double factor;
    int h;
    int v;
};

class MapScence : public QGraphicsScene
{
    Q_OBJECT
public:
    MapScence(QObject *parent = 0);
    MapScence(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);
    void setIsMainDisplay(bool is);
    void setActive(bool active){this->active = active;}
    bool isActive(){return  active;}
    ~MapScence();
    void setBackImage(const QPixmap *image);
    void bindOption(AreaMapOption *option){this->option = option;}
    AreaMapOption* getOption(){return this->option;}
protected:
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

signals:
    void dropFinished(int i);
private:
    QGraphicsItem *currentItem;
    const QPixmap *bgImage;
    bool isMainDisplay;
    bool active;
    AreaMapOption *option;
public slots:
    void removeCurrentItem();
};

#endif // MAPSCENCE_H
