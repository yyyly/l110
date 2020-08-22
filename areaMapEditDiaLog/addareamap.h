#ifndef ADDAREAMAP_H
#define ADDAREAMAP_H

#include <QDialog>
#include <QList>
#include <QListView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QImage>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QMenu>

#include "mylistview.h"
#include "mapscence.h"
#include "mymodel.h"
#include "pixmapitem.h"




class EditWidget : public QWidget
{
    Q_OBJECT
public:
    EditWidget(QWidget *parent = 0,QString name = QString());
    QLabel *label;
    QLineEdit *edit;
    QPushButton *openImageButton;
    QPushButton *okButton;
    QPushButton *cancelButton;
public slots:
    void eidtFinished();
};

class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomView(QWidget *parent = nullptr);
    void setOption(AreaMapOption *o);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
    bool viewportEvent(QEvent *event);
    void wheelEvent(QWheelEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    //void drawBackground(QPainter *painter, const QRectF &rect);


    QSize sizeHint() const;
private:
    QMenu *menu;
    double factor;
    AreaMapOption *option;
public slots:
    void saveFactor();

};

class AddAreaMap : public QDialog
{
    Q_OBJECT
public:
    AddAreaMap(const QList<int> &list, QWidget *parent = nullptr,AreaMapOption *o = nullptr);
private:
    QList<int> list;
    MyListView *listView;
    MyModel *model;
    CustomView *mapView;
    MapScence *scence = nullptr;
    EditWidget *editwidget;
    QFileDialog *fileDialog;
    QImage *image;
    AreaMapOption *option;
    QString imagePath;
    QList<QGraphicsItem *> startItemList;//编辑的时候获取已有的项
private:
    void assignAreaMpOption();
    bool listContaisItem(const QList<QGraphicsItem *> &list, QGraphicsItem *&item);
    bool isNew;
protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void newImageOpened(const QString imageName);
    void saveOption();
signals:
    void newAreaMapOption(AreaMapOption option);
    void updateAreaMapOption(AreaMapOption *option);
};



#endif // ADDAREAMAP_H
