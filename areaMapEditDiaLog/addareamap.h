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

#include "mylistview.h"
#include "mapscence.h"
#include "mymodel.h"
#include "pixmapitem.h"


struct AreaMapOption
{
public:
    AreaMapOption(){}
    QString areaMapName;
    QString backIamgePath;
    QList<AreaOption> areaList;
};

class EditWidget : public QWidget
{
public:
    EditWidget(QWidget *parent = 0,QString name = QString());
    QLabel *label;
    QLineEdit *edit;
    QPushButton *openImageButton;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

class CustomView : public QGraphicsView
{
public:
    CustomView(QWidget *parent = nullptr):
        QGraphicsView(parent)
    {}
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    void showEvent(QShowEvent *event);
    bool viewportEvent(QEvent *event);
    void wheelEvent(QWheelEvent *event);
    //void drawBackground(QPainter *painter, const QRectF &rect);


    QSize sizeHint() const;

};

class AddAreaMap : public QDialog
{
    Q_OBJECT
public:
    AddAreaMap(const QList<int> &list, QWidget *parent = 0,AreaMapOption *o = nullptr);
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
