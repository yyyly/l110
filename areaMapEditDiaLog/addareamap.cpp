#include "addareamap.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QBrush>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>


EditWidget::EditWidget(QWidget *parent, QString name)
{
    label = new QLabel("分区名称");
    edit = new QLineEdit(this);
    if(!name.isEmpty())
    {
        edit->setText(name);
    }
    openImageButton = new QPushButton("打开图片");
    okButton = new QPushButton("保存");
    cancelButton = new QPushButton("取消");
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(label);
    hLayout->addWidget(edit);
    hLayout->addWidget(openImageButton);
    hLayout->addStretch();
    hLayout->addWidget(okButton);
    hLayout->addWidget(cancelButton);
    setLayout(hLayout);

}



void CustomView::mousePressEvent(QMouseEvent *event)
{
    //QPoint pos = event->pos();
    //QPointF pos1 = mapToScene(pos);
    //qDebug() << pos1 << endl;
    QGraphicsView::mousePressEvent(event);
}

void CustomView::mouseReleaseEvent(QMouseEvent *e)
{
   QGraphicsView::mouseReleaseEvent(e);
}

void CustomView::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
}

bool CustomView::viewportEvent(QEvent *event)
{
    QGraphicsView::viewportEvent(event);
    return false;
}

void CustomView::wheelEvent(QWheelEvent *event)
{
    //scale(0.75,0.75);
    event->accept();
}

QSize CustomView::sizeHint() const
{
    return QWidget::sizeHint();
}

AddAreaMap::AddAreaMap(const QList<int> &list, QWidget *parent, AreaMapOption *o)
    :QDialog(parent),
      option(o)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAutoFillBackground(false);
    setFixedSize(800,500);
    this->list = list;
    listView = new MyListView();
    listView->setFixedWidth(150);
    model = new MyModel(list);
    listView->setModel(model);
    mapView = new CustomView();
    mapView->setDragMode(QGraphicsView::ScrollHandDrag);
    mapView->setAutoFillBackground(false);
    editwidget = new EditWidget(this);
    QVBoxLayout *vLayout= new QVBoxLayout();
    vLayout->addWidget(mapView);
    vLayout->addWidget(editwidget);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    hLayout->addWidget(listView);
    hLayout->addLayout(vLayout);
    //如果是编辑防区信息，添加防区图和防区热点
    if(option != nullptr)
    {
        QImage image(option->backIamgePath);
        scence = new MapScence();
        scence->setSceneRect(0,0,image.width(),image.height());
        scence->setBackgroundBrush(QBrush(image));
        mapView->setScene(scence);
        mapView->setSceneRect(0,0,image.width(),image.height());
        mapView->setAlignment(Qt::AlignCenter);
        mapView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        AreaOption area;
        foreach (area, option->areaList) {
            PixmapItem* item = new PixmapItem(area);
            item->setAlarmName(QString("%1防区").arg(item->getNum()));
            item->setMovealble(true);
            scence->addItem(item);
        }
        editwidget->edit->setText(option->areaMapName);
        startItemList = scence->items();
        connect(scence,SIGNAL(dropFinished(int)),model,SLOT(remListVaule(int)));
        connect(listView,SIGNAL(dropFinished()),scence,SLOT(removeCurrentItem()));
    }

    fileDialog = new QFileDialog(this,"打开图片","./images","images(*png *jpg *bmp)");

    connect(listView,SIGNAL(addItem(int)),model,SLOT(addItem(int)));
    connect(editwidget->openImageButton,SIGNAL(clicked(bool)),fileDialog,SLOT(exec()));
    connect(fileDialog,SIGNAL(fileSelected(QString)),this,SLOT(newImageOpened(QString)));
    connect(editwidget->okButton,SIGNAL(clicked(bool)),this,SLOT(saveOption()));
}

void AddAreaMap::newImageOpened(const QString imageName)
{
    //if(image)
        //delete image;
    imagePath = imageName;
    scence = new MapScence();
    image = new QImage(imageName);
    scence->setSceneRect(0,0,image->width(),image->height());
    scence->setBackgroundBrush(QBrush(*image));
    mapView->setScene(scence);
    mapView->setSceneRect(0,0,image->width(),image->height());
    mapView->setAlignment(Qt::AlignCenter);
    mapView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    connect(scence,SIGNAL(dropFinished(int)),model,SLOT(remListVaule(int)));
    connect(listView,SIGNAL(dropFinished()),scence,SLOT(removeCurrentItem()));
}


void AddAreaMap::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
}

void AddAreaMap::saveOption()
{
    QSqlTableModel m;
    if(editwidget->edit->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","请输入防区图名称");
        return;
    }
    if(scence == nullptr)
    {
        QMessageBox::warning(this,"提示","未添加防区图");
        return;
    }
    if(option == nullptr)//说明是新添加
    {
       option = new AreaMapOption();//注意释放内存
       option->backIamgePath = imagePath;
    }
    option->areaMapName = editwidget->edit->text();


    m.setTable("areaMap");
    m.insertRows(0,1);
    if(m.setData(m.index(0,0),option->areaMapName))
    {

    }
    m.setData(m.index(0,1),option->backIamgePath);
    m.submit();
    QList<QGraphicsItem *> itemList = scence->items();
    QList<QGraphicsItem *> addItemList;//itemList - startItemList，在startItemLisr中有itemList不存在的项
    QGraphicsItem *item;
    //在itemList中去除掉存在于startItemList中的元素
    foreach (item, itemList) {
        if(!listContaisItem(startItemList,item)){
            addItemList << item;
        }

    }
    m.setTable("pointOnMap");
    QList<int> alarmList = model->getBuffetList();//新添加的，还有已有的项删除后又添加的
    QSqlQuery query;
    foreach (item, addItemList) {//全部是新添加的
        PixmapItem *pixmapItem = dynamic_cast<PixmapItem *>(item);
        int n = pixmapItem->getNum();
        if(alarmList.contains(n))
        {
            option->areaList << pixmapItem->getAreaOption();
            query.exec(QString("UPDATE alarmImfor SET ON_SCENCE = 1 WHERE ALARM_NUM = %1").arg(n));
            m.insertRows(0,1);
            m.setData(m.index(0,0),option->areaMapName);
            m.setData(m.index(0,1),pixmapItem->getNum());
            m.setData(m.index(0,2),pixmapItem->getAreaOption().type);
            m.setData(m.index(0,3),pixmapItem->getAreaOption().pos.x());
            m.setData(m.index(0,4),pixmapItem->getAreaOption().pos.y());
            m.submit();
        }

    }
    //添加之前已有的项，且现在不存在的项，删除掉
    foreach (item, startItemList) {
        if(!listContaisItem(itemList,item))
        {
            PixmapItem *pixmapItem = dynamic_cast<PixmapItem *>(item);
            option->areaList.removeOne(pixmapItem->getAreaOption());
            int n = pixmapItem->getNum();
            query.exec(QString("UPDATE alarmImfor SET ON_SCENCE = 0 WHERE ALARM_NUM = %1").arg(n));
            query.exec(QString("DELETE FROM pointOnMap WHERE NUM = %1").arg(n));
        }
        else { //添加之前已有的项，且现在存在的项，更新
            PixmapItem *pixmapItem = dynamic_cast<PixmapItem *>(item);
            int n = pixmapItem->getNum();
            query.exec(QString("UPDATE pointOnMap SET POS_X = %1,POS_Y = %2 WHERE NUM = %3 ").arg(item->x()).arg(item->y()).arg(n));
            n = option->areaList.indexOf(pixmapItem->getAreaOption());
            if(n >= 0 && n < option->areaList.size())
            {
               option->areaList[n] =  pixmapItem->getAreaOption();
            }
        }

    }


    alarmList.clear();
    emit newAreaMapOption(*option);
    emit updateAreaMapOption(option);
    this->close();
}

bool AddAreaMap::listContaisItem(const QList<QGraphicsItem *> &list,QGraphicsItem *&item)
{
    PixmapItem *pItem1 = dynamic_cast<PixmapItem *>(item);
    QGraphicsItem *sItem;
    foreach (sItem, list) {
       PixmapItem *pItem2 = dynamic_cast<PixmapItem *>(sItem);
       if(pItem1->getNum() == pItem2->getNum())
       {
           return true;
       }
    }
    return false;
}



