  #include "areamap.h"
#include <QPixmap>
#include <QPalette>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QImage>
#include <QSqlQuery>
#include <QStatusBar>
#include <QMessageBox>

#include "globdata.h"
AreaMap::AreaMap(QWidget *parent)
    : QWidget(parent),
      addArea(new QPushButton(this)),
      dockWidget(new CDockWidget),
      view(new CustomView),
      scence(new MapScence),
      screen(new Screen(1,view)),
      bar(new CStatuBar(view)),
      playId(-1),
      currentPlayNum(-1)
{
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    view->setStyleSheet("border:none");
    //scence->setSceneRect(0,0,1024,960);
    view->setScene(scence);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dockWidget->setStyleSheet("border:none");
    dockWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    dockWidget->setMinimumWidth(150);
    QSplitter *splitter = new QSplitter;
    splitter->addWidget(view);
    splitter->addWidget(dockWidget);
    QList<int> list;
    list << 1000 << 150;
    splitter->setSizes(list);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->setSpacing(0);
    hlayout->addWidget(splitter);
    setLayout(hlayout);
    setObjectName("areaMap");

    QSqlQuery query;
    query.exec("SELECT * FROM areaMap");
    AreaMapOption option;
    AreaOption areaOption;
    while (query.next()) {
        option.areaMapName = query.value(0).toString();
        option.backIamgePath = query.value(1).toString();
        dockWidget->addTab(option.areaMapName);
        QSqlQuery query1;
        query1.exec(QString("SELECT * FROM pointOnMap WHERE NAME = '%1'").arg(option.areaMapName));
        while(query1.next())
        {
            areaOption.num = query1.value(1).toInt();
            areaOption.type = (AlarmType)query1.value(2).toInt();
            areaOption.pos.rx() = query1.value(3).toInt();
            areaOption.pos.ry() = query1.value(4).toInt();
            option.areaList.append(areaOption);
        }
        query1.clear();
        areaMapOptionList << option;
        option.areaList.clear();
    }

    option = areaMapOptionList.value(0);
    QImage image(option.backIamgePath);
    scence->setSceneRect(0,0,image.width(),image.height());
    scence->setBackgroundBrush(QBrush(image));
    view->centerOn(image.width()/2,image.height()/2);
    AreaOption area;
    foreach (area, option.areaList) {
        PixmapItem* item = new PixmapItem(area);
        item->setAlarmName(QString("%1防区").arg(item->getNum()));
        item->setMovealble(false);
        scence->addItem(item);
    }
    screen->flag = 1;//设置这个是为了操作screen界面时不影响全局变量
    screen->hide();

    bar->hide();

    connect(dockWidget,SIGNAL(tabChanged(int)),this,SLOT(areaMapChanged(int)));
    connect(dockWidget->addButton,SIGNAL(clicked(bool)),this,SLOT(addArea_Clicked()));
    connect(dockWidget->editButton,SIGNAL(clicked(bool)),this,SLOT(editArea_Clicked()));
    connect(dockWidget->deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteArea_clicked()));
    connect(screen,SIGNAL(cameraHide()),this,SLOT(CloseScreen()));

}

void AreaMap::addArea_Clicked()
{
    //初始化noBindList;
    noBindList.clear();
    QSqlQuery query;
    query.exec(QString("SELECT ALARM_NUM FROM alarmImfor WHERE ON_SCENCE = 0"));
    while (query.next()) {
        int i = query.value(0).toInt();
        noBindList << i;
    }
    addAreaDialog = new AddAreaMap(noBindList);
    connect(addAreaDialog,SIGNAL(newAreaMapOption(AreaMapOption)),this,SLOT(newAreaMap(AreaMapOption)));
    addAreaDialog->exec();
}

void AreaMap::editArea_Clicked()
{
    noBindList.clear();
    QSqlQuery query;
    query.exec(QString("SELECT ALARM_NUM FROM alarmImfor WHERE ON_SCENCE = 0"));
    while (query.next()) {
        int i = query.value(0).toInt();
        noBindList << i;
    }
    int row = dockWidget->tabList->currentRow();
    QList<AreaMapOption>::iterator it = areaMapOptionList.begin();
    for(int j = 0;j < row;j++)
    {
        if(it != areaMapOptionList.end())
        {
            it++;
        }
    }
    addAreaDialog = new AddAreaMap(noBindList,this,&(*it));
    connect(addAreaDialog,SIGNAL(updateAreaMapOption(AreaMapOption *)),this,SLOT(updateAreaMap(AreaMapOption *)));
    addAreaDialog->exec();
}

void AreaMap::deleteArea_clicked()
{
    //提醒客户这是删除操作，需要客户再次确认
    QMessageBox msgBox;
    msgBox.setText("确定删除此防区图");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    int row = dockWidget->tabList->currentRow();
    QList<AreaMapOption>::iterator it = areaMapOptionList.begin();
    //定位到需要删除的位置
    for(int j = 0;j < row;j++)
    {
        if(it != areaMapOptionList.end())
        {
            it++;
        }
    }
    //更新数据库数据
    QSqlQuery query;
    AreaOption option;
    foreach (option, it->areaList) {
        int n = option.num;
        query.exec(QString("UPDATE alarmImfor SET ON_SCENCE = 0 WHERE ALARM_NUM = %1").arg(n));
        query.exec(QString("DELETE FROM pointOnMap WHERE NUM = %1").arg(n));
    }
    //这句一开始写进了循环里面，导致如果添加了没有热点的防区图后点删除实际是不能更新数据库的，并且如果添加了多个热点，则
    //多次删除数据库中的同一条记录
    query.exec(QString("DELETE FROM areaMap WHERE NAME = '%1'").arg(it->areaMapName));
    dockWidget->deleteTab(row);//这句执行后会去更新防区图
    areaMapOptionList.removeAt(row);//这两句顺序不能反
}

void AreaMap::CloseScreen()
{
    currentPlayNum = -1;
    CameraDeviceImf *imf = getScreen()->getCameraDeviceImf();
    CameroNet *cameroNet = CameroNet::getInstance();
    int r = cameroNet->stopPlay(*imf,getScreen());
}


void AreaMap::resizeEvent(QResizeEvent *event)
{

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background,QPixmap(":/image/ground.png").scaled(this->size()));
    setPalette(palette);
    addArea->setGeometry(this->width()-addArea->width()-5,0,addArea->width(),addArea->height());
}

void AreaMap::newAreaMap(AreaMapOption option)
{
    areaMapOptionList << option;
    dockWidget->addTab(option.areaMapName);
}

void AreaMap::areaMapChanged(int n)
{
    AreaMapOption option = areaMapOptionList.value(n);
    updateAreaMap(&option);
}

void AreaMap::updateAreaMap(AreaMapOption *option)
{
    scence->clear();
    screen->hide();
    bar->hide();
    AreaOption area;
    QImage image(option->backIamgePath);
    scence->setSceneRect(0,0,image.width(),image.height());
    scence->setBackgroundBrush(QBrush(image));
    view->centerOn(image.width()/2,image.height()/2);
    foreach (area, option->areaList) {
        PixmapItem* item = new PixmapItem(area);
        item->setAlarmName(QString("%1防区").arg(item->getNum()));
        item->setMovealble(false);
        scence->addItem(item);
        if(currentPlayNum != -1)//说明有一个防区图上正在播放视频
        {
            if(currentPlayNum == item->getNum())
            {
                screen->show();
                view->centerOn(item->getAreaOption().pos);
            }

        }
    }
}

void AreaMap::alarmNumChecked(int num)
{
    if(num == currentPlayNum)//这个防区已经有在播放，直接退出
    {
        return;
    }
    CameroNet *cameroNet = CameroNet::getInstance();
    if(currentPlayNum != -1)//说明前一次有播放，先停止播放
    {
        if(playId != -1)
        {
            cameroNet->stopPlay(playId);//这个接口在设备离线的时候耗时较长，会导致主界面操作卡顿，需要用多线程方法处理。
            screen->hide();
        }
        currentPlayNum = -1;
    }
    bar->hide();
    AreaMapOption mapOption;
    AreaOption option;
    bool sign = false;
    int i = 0;
    int j = 0;
    foreach (mapOption, areaMapOptionList) {
        j = 0;
        foreach (option, mapOption.areaList) {
            j++;
           if(option.num == num)
           {
               sign = true;
               break;
           }
        }
        if(sign)
        {
            break;
        }
        i++;
    }
    if(i == areaMapOptionList.count() && j == mapOption.areaList.count())
    {
        screen->hide();
        bar->showMessage("防区未添加到防区图",3000);
        int w = (this->width() - bar->width()) / 2 - bar->width();
        int h = 20;
        bar->setGeometry(w,h,160,40);
        return;
    }
    //areaMapChanged(i);
    dockWidget->tabList->setCurrentRow(i);
    view->centerOn(option.pos);
    QSqlQuery query;
    query.exec(QString("SELECT serial,channal_num FROM vedioLink WHERE ALARM_NUM = %1").arg(option.num));
    if(query.next())
    {
        CameraDeviceImf  imf = deviceImf[query.value(0).toInt()];
        if(imf.luserId != -1 && imf.lineState == Camero::OnLine)
        {
            QPointF pos = view->mapFromScene(option.pos);
            screen->setGeometry(pos.x()+35,pos.y(),280,210);
            screen->show();
            playId = cameroNet->realPlay(&imf,1,*screen);
            currentPlayNum = num;
        }
        else {
            //如果上一次通道有成功播放，需要先停止播放。
            CameroNet *cameroNet = CameroNet::getInstance();
            cameroNet->stopPlay(playId);
            screen->hide();
            QPointF pos = view->mapFromScene(option.pos);
            bar->setGeometry(pos.x()+35,pos.y(),80,20);
            bar->showMessage("视频主机不在线",3000);
        }

    }
    else
    {
        screen->hide();
        QPointF pos = view->mapFromScene(option.pos);
        bar->setGeometry(pos.x()+35,pos.y(),80,20);
        bar->showMessage("未绑定视频",3000);
        return;
    }


}

void AreaMap::closeChannal()
{
    CameroNet *cameroNet = CameroNet::getInstance();
    cameroNet->stopPlay(playId);
    screen->hide();
}

CDockWidget::CDockWidget(QWidget *parent)
    : QWidget(parent),
      button(new QPushButton),
      label(new QLabel),
      tabList(new QListWidget),
      addButton(new QPushButton),
      deleteButton(new QPushButton),
      editButton(new QPushButton)
      /*,
      vLabel(new QLabel),
      spreadButton(new QPushButton)*/

{
    //setFixedWidth(100);
    button->setFixedSize(20,20);
    button->setObjectName("foldButton");
    button->setIcon(QIcon(":/image/shouqi.png"));
    button->hide();
    addButton->setFixedSize(30,20);
    addButton->setObjectName("foldButton");
    addButton->setIcon(QIcon(":/image/tianjia.png"));
    addButton->setToolTip("添加防区图");
    editButton->setFixedSize(20,20);
    editButton->setObjectName("foldButton");
    editButton->setIcon(QIcon(":/image/edit.png"));
    editButton->setToolTip("编辑防区图");
    deleteButton->setFixedSize(20,20);
    deleteButton->setObjectName("foldButton");
    deleteButton->setIcon(QIcon(":/image/delete.png"));
    deleteButton->setToolTip("删除防区图");
    label->setText("  防区图列表");
    label->setObjectName("areaTitle");
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->setSpacing(0);
    hlayout->addWidget(button);
    hlayout->addWidget(label);
    hlayout->addStretch();
    hlayout->addWidget(addButton);
    QWidget *widget = new QWidget;
    widget->setObjectName("titleWidget");
    widget->setLayout(hlayout);
    QHBoxLayout *bHlayout = new QHBoxLayout;
    bHlayout->setMargin(0);
    bHlayout->setSpacing(6);
    bHlayout->addWidget(deleteButton);
    bHlayout->addWidget(editButton);
    bHlayout->addStretch();
    QWidget *bWidget = new QWidget;
    bWidget->setObjectName("titleWidget");
    bWidget->setLayout(bHlayout);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addWidget(widget);
    vLayout->addWidget(tabList);
    vLayout->addWidget(bWidget);
    /*vLabel->setText("防区图列表");
    vLabel->setWordWrap(true);
    vLabel->setAlignment(Qt::AlignTop);
    QVBoxLayout *vLayout1 = new QVBoxLayout(this);
    vLayout1->addWidget(spreadButton);
    vLayout1->addWidget(vLabel);
    spreadButton->hide();
    vLabel->hide();

    QHBoxLayout *mHBoxLayout = new QHBoxLayout;
    mHBoxLayout->addLayout(vLayout);
    mHBoxLayout->addLayout(vLayout1);
    this->setLayout(mHBoxLayout);*/

    connect(tabList,SIGNAL(currentRowChanged(int)),this,SLOT(itemChanged(int)));
    connect(button,SIGNAL(clicked(bool)),this,SLOT(draw()));
}

void CDockWidget::addTab(const QString &str)
{
  tabList->addItem(str);
}

void CDockWidget::deleteTab(int row)
{
    tabList->takeItem(row);
}


void CDockWidget::itemChanged(int row)
{
    emit tabChanged(row);
}

void CDockWidget::draw()
{
    tabList->hide();
    button->hide();
    label->hide();
    addButton->hide();

    spreadButton->show();
    vLabel->show();
}

