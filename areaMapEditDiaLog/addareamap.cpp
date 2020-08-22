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
#include <QScrollBar>


EditWidget::EditWidget(QWidget *parent, QString name)
{
    label = new QLabel("分区名称");
    edit = new QLineEdit(this);
    if(!name.isEmpty())
    {
        edit->setText(name);
    }
    openImageButton = new QPushButton("设置防区图");
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
    connect(edit,&QLineEdit::editingFinished,this,&EditWidget::eidtFinished);
}

void EditWidget::eidtFinished()
{
    QSqlQuery query;
    QString name = edit->text();
    QString sql = QString("SELECT NAME FROM areaMap WHERE NAME = \"%1\"").arg(name);
    query.exec(sql);
    if(query.next())
    {
        QMessageBox::warning(this,"提示","防区名已存在");
        return;
    }
}

CustomView::CustomView(QWidget *parent)
    :QGraphicsView(parent)
    ,menu(new QMenu(this))
    ,option(nullptr)
{
    QAction *action = menu->addAction("保存缩放比列");
    connect(action,&QAction::triggered,this,&CustomView::saveFactor);
}

void CustomView::setOption(AreaMapOption *o)
{
    if(o == nullptr)
    {
        option = o;
        return;
    }
    option = o;
    QSqlQuery query;
    QString sql = QString("SELECT factor FROM areaMap WHERE name = '%1'").arg(option->areaMapName);
    query.exec(sql);
    if(query.next())
    {
        factor = query.value(0).toDouble();
    }
    else
        factor = 1;

}

void CustomView::saveFactor()
{
    QSqlQuery query;
    QString sql = QString("UPDATE areaMap SET factor = %1 WHERE name = '%2'")
            .arg(factor).arg(option->areaMapName);
    query.exec(sql);
    sql = QString("UPDATE areaMap SET rect_x = %1,rect_y = %2 WHERE name = '%3'")
            .arg(this->horizontalScrollBar()->value()).arg(this->verticalScrollBar()->value()).arg(option->areaMapName);
    option->factor = factor;
    option->h = this->horizontalScrollBar()->value();
    option->v = this->verticalScrollBar()->value();
    query.exec(sql);

}

void CustomView::contextMenuEvent(QContextMenuEvent *event)
{
    if(option == nullptr)
        return;
    menu->exec(event->globalPos());
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

void CustomView::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QPen(Qt::blue,4));
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(2,2,20,20,0,0);
    QGraphicsView::paintEvent(event);
}

void CustomView::wheelEvent(QWheelEvent *event)
{
    if(option == nullptr)
        return;
    double numDegrees = -event->delta() / 8.0;
    double numsteps = numDegrees / 15.0;
    double f = std::pow(1.125,numsteps);
    scale(f,f);
    factor = factor * f;
    //option->factor = factor;
}

QSize CustomView::sizeHint() const
{
    return QWidget::sizeHint();
}

AddAreaMap::AddAreaMap(const QList<int> &list, QWidget *parent, AreaMapOption *o)
    :QDialog(parent),
      scence(nullptr),
      option(o),
      isNew(true)
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
    if(o != nullptr)
    {
        isNew = false;
        scence = new MapScence(this);
        scence->setSceneRect(0,0,option->backImage.width(),option->backImage.height());
        scence->setBackImage(&option->backImage);
        mapView->setSceneRect(0,0,option->backImage.width(),option->backImage.height());
        mapView->setAlignment(Qt::AlignCenter);
        mapView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        mapView->setScene(scence);
        mapView->setOption(option);
        AreaOption area;
        foreach (area, option->areaList) {
            PixmapItem* item = new PixmapItem(area);
            item->setAlarmName(QString("%1防区").arg(item->getNum()));
            item->setMovealble(true);
            scence->addItem(item);
        }
        editwidget->edit->setText(option->areaMapName);
        startItemList = scence->items();
        imagePath = option->backIamgePath;
        mapView->scale(option->factor,option->factor);
        connect(scence,SIGNAL(dropFinished(int)),model,SLOT(remListVaule(int)));
        connect(listView,SIGNAL(dropFinished()),scence,SLOT(removeCurrentItem()));
    }
    else {
        option = new AreaMapOption();
        option->factor = 1;
    }
    fileDialog = new QFileDialog(this,"打开图片","./images","images(*png *jpg *bmp)");

    connect(listView,SIGNAL(addItem(int)),model,SLOT(addItem(int)));
    connect(editwidget->openImageButton,SIGNAL(clicked(bool)),fileDialog,SLOT(exec()));
    connect(fileDialog,SIGNAL(fileSelected(QString)),this,SLOT(newImageOpened(QString)));
    connect(editwidget->okButton,SIGNAL(clicked(bool)),this,SLOT(saveOption()));
}

void AddAreaMap::newImageOpened(const QString imageName)
{
    if(imagePath == imageName)
    {
        return;
    }
    if(scence == nullptr)
    {
        scence = new MapScence();
        connect(scence,SIGNAL(dropFinished(int)),model,SLOT(remListVaule(int)));
        connect(listView,SIGNAL(dropFinished()),scence,SLOT(removeCurrentItem()));
    }
    imagePath = imageName;
    option->backImage = QPixmap(imagePath);
    scence->setSceneRect(0,0,option->backImage.width(),option->backImage.height());
    scence->setBackImage(&option->backImage);
    mapView->setSceneRect(0,0,option->backImage.width(),option->backImage.height());
    mapView->setAlignment(Qt::AlignCenter);
    mapView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    mapView->setScene(scence);
    mapView->setOption(option);
    //mapView->scale(0.1,0.1);
    QList<QGraphicsItem *>  itemList = scence->items();
    int x = 30;
    foreach (auto item, itemList) {
        item->setPos(x,18);
        x = x + 40;
        PixmapItem *pixmapItem = dynamic_cast<PixmapItem *>(item);
        pixmapItem->getAreaOption().pos.rx() = x;
        pixmapItem->getAreaOption().pos.ry() = 18;
    }
    //mapView->centerOn(-100,-100);
    mapView->centerOn(0,0);

}


void AddAreaMap::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
}

void AddAreaMap::saveOption()
{
    QSqlQuery query;
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


    //option->factor = mapView->matrix().m11();
    if(isNew)
    {

        if(!imagePath.isEmpty())
        {
            option->backIamgePath = imagePath;
        }
        m.setTable("areaMap");
        m.insertRows(0,1);
        m.setData(m.index(0,0),editwidget->edit->text());
        m.setData(m.index(0,1),imagePath);
        m.setData(m.index(0,2),1);
        m.submit();
    }
    else
    {
        QString sql = QString("UPDATE areaMap SET PATH = \"%1\",NAME = \"%2\" WHERE NAME = \"%3\"")
                .arg(imagePath).arg(editwidget->edit->text()).arg(option->areaMapName);

        query.exec(sql);
    }
    option->areaMapName = editwidget->edit->text();
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

    this->scence->bindOption(option);
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



