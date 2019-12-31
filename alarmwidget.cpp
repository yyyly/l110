#include "alarmwidget.h"

#include<QDebug>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSplitter>
#include<QBrush>
#include<QPixmap>
#include<QColor>
#include<QSizePolicy>
#include<QDebug>
#include<QDateTime>
#include<QHeaderView>
#include<QList>
#include<QJsonParseError>
#include<QJsonDocument>
#include<QDir>
#include<QSound>
#include<QStandardItemModel>
#include<QScrollBar>
#include"globdata.h"
#include"z1801p.h"
#include "bombScreen/bombscreen.h"
#include "globaloption.h"
#include "mytimer.h"



enum{
    ZONE_NUM,
    ZONE_NAME,
    PART_NUM,
    PART_NAME,
    ZONE_TYPE,
    EVENT_TIME,
    REPORT_TIME,
    CHARG1_NAME,
    CHARG1_PHONE,
    CHARG2_NAME,
    CHARG2_PHONE,
    IS_HANDLE,
    HANDLE_PERSON,
    HANDLE_OPINION,
    NOTICE_PERSON,
    NOTICE_PHONE,
    ALARM_CLASS,
    VEDIO_PATH
};

enum ALARMING_CLASS
{
    ALARMING_WARMING,
    DEVICE_SYSTEM
};

AlarmWidget::AlarmWidget(QWidget *parent)
    : QWidget(parent),
      devPanel(new QTreeView(this)),
      imforView(new QTableView(this)),
      z1801(new Z1801(this)),
      ruleDialog(new RuleWidget(this)),
      dataEntryDialog(new DataEntryDialog(this)),
      vediolLinkConfigDialog(new VedioLinkConfigDialog(this)),
      areaMap(new AreaMap(this)),
      statusBar(new QStatusBar(this)),
      contralPanel(new ContralPanel(this)),
      deviceSetWidget(new DeviceSetWidget(this,z1801)),
      controWidget(new ControlWidget(this)),
      currentAlarmNum(-1),
      soundPlayer(nullptr)
{
    //updateNickNmae(glob_user);
    InitAlarmDeviceImf();
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    statusBar->setFixedWidth(200);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setMargin(0);
    leftLayout->setSpacing(0);
    leftLayout->addWidget(contralPanel);
    leftLayout->addWidget(devPanel);
    //leftLayout->addStretch();
    leftLayout->addWidget(statusBar);
    imforView->setMinimumHeight(150);
    imforView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    //imforView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QSplitter *rightSplitter = new QSplitter(Qt::Vertical);
    rightSplitter->addWidget(areaMap);
    rightSplitter->addWidget(imforView);
    rightSplitter->setCollapsible(1,false);
    QList<int> sizeList;
    sizeList << 1750 << 150;
    rightSplitter->setSizes(sizeList);
    QVBoxLayout *rVBoxlayout = new QVBoxLayout();
    rVBoxlayout->setMargin(0);
    rVBoxlayout->addWidget(rightSplitter);
    rVBoxlayout->addWidget(controWidget);
    QHBoxLayout *midlayout = new QHBoxLayout();
    midlayout->setMargin(0);
    midlayout->setSpacing(0);
    midlayout->addLayout(leftLayout);
    midlayout->addLayout(rVBoxlayout);
    setLayout(midlayout);
    rulers = ruleDialog->getRulers();
    model = new CSqlTableMode(&rulers,this,QSqlDatabase());
    controWidget->setModel(model);
    controWidget->setView(imforView);
    model->setTable("alarm");
    model->setSort(REPORT_TIME,Qt::DescendingOrder);
    model->setHeaderData(ZONE_NUM,Qt::Horizontal,"防区编号");
    model->setHeaderData(ZONE_NAME,Qt::Horizontal,"防区名称");
    model->setHeaderData(PART_NUM,Qt::Horizontal,"分区编号");
    model->setHeaderData(PART_NAME,Qt::Horizontal,"分区名称");
    model->setHeaderData(ZONE_TYPE,Qt::Horizontal,"防区类型");
    model->setHeaderData(EVENT_TIME,Qt::Horizontal,"事件时间");
    model->setHeaderData(REPORT_TIME,Qt::Horizontal,"上报时间");
    model->setHeaderData(PART_NAME,Qt::Horizontal,"分区名称");
    model->setHeaderData(CHARG1_NAME,Qt::Horizontal,"第一联系人");
    model->setHeaderData(CHARG1_PHONE,Qt::Horizontal,"联系电话");
    model->setHeaderData(CHARG2_NAME,Qt::Horizontal,"第二联系人");
    model->setHeaderData(CHARG2_PHONE,Qt::Horizontal,"联系电话");
    model->setFilter("IS_HANDLE != 1 OR IS_HANDLE IS null");
    model->select();
    /*proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(ALARM_CLASS);*/
    imforView->setModel(model);
    imforView->setSelectionBehavior(QAbstractItemView::SelectRows);
    imforView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    imforView->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格不可编辑
    imforView->verticalHeader()->setVisible(false);//垂直表头不可见
    imforView->setColumnHidden(IS_HANDLE,true);
    imforView->setColumnHidden(HANDLE_PERSON,true);
    imforView->setColumnHidden(ALARM_CLASS,true);
    imforView->setColumnHidden(VEDIO_PATH,true);
    imforView->setColumnHidden(HANDLE_OPINION,true);
    imforView->setColumnHidden(NOTICE_PERSON,true);
    imforView->setColumnHidden(NOTICE_PHONE,true);
    imforView->resizeColumnToContents(EVENT_TIME);
    imforView->resizeColumnToContents(REPORT_TIME);
    //imforView->horizontalHeader()->swapSections(0,2);
    //imforView->horizontalHeader()->moveSection(2,0);

    //读取数据库防区信息，构建防区编号和防区名称的Map
    QSqlQuery query;
    updateMessing();
    //构建警情类型描述Map
    query.exec("SELECT ZONE_CODE,ZONE_TYPE FROM ruler");
    while(query.next())
    {
        int code = query.value(0).toInt();
        QString desc = query.value(1).toString();
        messingTypeMap[code] = desc;
    }
    ImforParse *dataParse = new ImforParse();
    Node *rootNode = dataParse->parse(alarmDeviceImf,false);
    devModel = new DeviceModel(this);
    devModel->setRootNode(rootNode);
    devPanel->setModel(devModel);
    devPanel->setItemDelegate(new NodeDelegate());
    devPanel->setFixedWidth(200);
    devPanel->header()->hide();
    connect(z1801,SIGNAL(getDeviceImfFinished(AlarmStatu *,int *)),this,SLOT(alarmDviceImf(AlarmStatu*,int*)));
    connect(z1801,SIGNAL(haveAlarmMessing(AlarmMessing)),this,SLOT(alarmMessing(AlarmMessing)));
    connect(z1801,SIGNAL(deviceLogin()),this,SLOT(deviceLog()));
    connect(z1801,SIGNAL(updatePartImfor(const PartImf &)),this,SLOT(updatePartImfor(const PartImf &)));
    connect(z1801,SIGNAL(barMessing(const QString &)),this,SLOT(showBarMessing(const QString &)));
    connect(ruleDialog,SIGNAL(rulerChange()),this,SLOT(rulerChanged()));
    connect(contralPanel,SIGNAL(setDevice()),deviceSetWidget,SLOT(show()));
    connect(contralPanel,SIGNAL(defence()),this,SLOT(deviceDefence()));
    connect(contralPanel,SIGNAL(disDefence()),this,SLOT(deviceDisDefence()));
    connect(imforView,SIGNAL(clicked(QModelIndex)),this,SLOT(alarmNumFromView(QModelIndex)));
    connect(this,SIGNAL(alarmNumCheck(int)),areaMap,SLOT(alarmNumChecked(int)));
    connect(controWidget,SIGNAL(stateChange(int)),this,SLOT(messingFilter(int)));
    connect(controWidget,SIGNAL(handleHappened()),areaMap,SLOT(closeChannal()));
    //BombScreen *screen = new BombScreen(,this);

    //screen->show();
}

void AlarmWidget::updateNickNmae(const QString &name)
{
    QSqlQuery query;
    query.exec(QString("SELECT NICKNAME FROM logImfor WHERE ACCOUNT = '%1'").arg(name));
    if(query.next())
    {
        glob_nickName = query.value(0).toString();
    }
}

void AlarmWidget::updateMessing()
{
    QSqlQuery query;
    query.exec("SELECT * FROM alarmImfor");
    while(query.next())
    {
        int num = query.value(0).toInt();
        RelatedMessingByNum messing;
        messing.name = query.value(1).toString();
        messing.person_1_name = query.value(2).toString();
        messing.person_1_phone = query.value(3).toString();
        messing.person_2_name = query.value(4).toString();
        messing.person_2_phone = query.value(5).toString();
        alarmMessingMap[num] = messing;
    }

    //构建分区名称Map
    query.exec("SELECT * FROM partImfor");
    while(query.next())
    {
        int num = query.value(0).toInt();
        RelatedMessingByNum messing;
        messing.name = query.value(1).toString();
        messing.person_1_name = query.value(2).toString();
        messing.person_1_phone = query.value(3).toString();
        messing.person_2_name = query.value(4).toString();
        messing.person_2_phone = query.value(5).toString();
        partMessingMap[num] = messing;
    }


}

void AlarmWidget::alarmMessing(AlarmMessing messing)
{
    //if(messing.type != AlarmType::STONE_ALARM)
        //return;
    int row = 0;
    int alarmId = messing.aLarmID;
    int partId = messing.zoneId;
    QString type = messingTypeMap.value(messing.type);
    if(type.isEmpty())
        return;
    model->insertRows(row,1);
    if(messing.type == 0x135a)
    {
        QString key = QString("key-%1").arg(alarmId);
        model->setData(model->index(row,ZONE_NUM),key);
    }
    else {
        model->setData(model->index(row,ZONE_NUM),alarmId);
        model->setData(model->index(row,ZONE_NAME),alarmMessingMap.value(alarmId).name);
    }
    model->setData(model->index(row,ZONE_TYPE),type);
     model->setData(model->index(row,PART_NUM),partId);
    model->setData(model->index(row,PART_NAME),partMessingMap.value(partId).name);
    model->setData(model->index(row,EVENT_TIME),messing.time.toString("yyyy-MM-dd hh:mm:ss"));
    model->setData(model->index(row,REPORT_TIME),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    QString alarm_person_1_name = alarmMessingMap.value(alarmId).person_1_name;
    QString alarm_person_2_name = alarmMessingMap.value(alarmId).person_2_name;
    QString alarm_person_1_phone = alarmMessingMap.value(alarmId).person_1_phone;
    QString alarm_person_2_phone = alarmMessingMap.value(alarmId).person_2_phone;
    if(!alarm_person_1_name.isEmpty() || !alarm_person_1_phone.isEmpty() || !alarm_person_2_name.isEmpty()
            || !alarm_person_2_phone.isEmpty())
    {
        model->setData(model->index(row,CHARG1_NAME),alarm_person_1_name);
        model->setData(model->index(row,CHARG1_PHONE),alarm_person_1_phone);
        model->setData(model->index(row,CHARG2_NAME),alarm_person_2_name);
        model->setData(model->index(row,CHARG2_PHONE),alarm_person_2_phone);

    }
    else {
        alarm_person_1_name = partMessingMap.value(partId).person_1_name;
        alarm_person_2_name = partMessingMap.value(partId).person_2_name;
        alarm_person_1_phone = partMessingMap.value(partId).person_1_phone;
        alarm_person_2_phone = partMessingMap.value(partId).person_2_phone;
        if(!alarm_person_1_name.isEmpty() || !alarm_person_1_phone.isEmpty() || !alarm_person_2_name.isEmpty()
                || !alarm_person_2_phone.isEmpty())
        {
            model->setData(model->index(row,CHARG1_NAME),alarm_person_1_name);
            model->setData(model->index(row,CHARG1_PHONE),alarm_person_1_phone);
            model->setData(model->index(row,CHARG2_NAME),alarm_person_2_name);
            model->setData(model->index(row,CHARG2_PHONE),alarm_person_2_phone);
        }
    }
    //播放声音
    Ruler ruler = rulers.value(type);
    if(soundPlayer != nullptr)
    {
        soundPlayer->stop();
        delete soundPlayer;
    }
    soundPlayer = new QSound(ruler.soundAddress);
    soundPlayer->setLoops(ruler.playTimes);
    soundPlayer->play();

    //播放绑定的视频
    if(type == "24小时有声报警" || type == "出/入口报警" || type == "24小时无声报警" || type == "防盗防区"
            || type == "火警防区" || type == "紧急报警")
    {
        //areaMap->alarmNumChecked(alarmId);
        Screen *pre = areaMap->getPreScreen();
        if(pre)
        {
            pre->hide();
        }
        model->setData(model->index(row,ALARM_CLASS),ALARMING_WARMING);
        QSqlQuery query;
        query.exec(QString("SELECT serial,channal_num FROM vedioLink WHERE ALARM_NUM = %1").arg(alarmId));
        if(query.next())
        {
            CameraDeviceImf  imf = deviceImf[query.value(0).toInt()];
            CameroNet *cameroNet = CameroNet::getInstance();
            if(imf.luserId != -1)
            {
                //先判断防区关联的视频有没有在播放，如果在播放则先停止播放
                Screen *playedScreen = areaMap->getScreenByNum(alarmId);
                if(playedScreen)
                {
                    cameroNet->stopPlay(*playedScreen->getCameraDeviceImf(),playedScreen);
                    playedScreen->setPlayState(Screen::UNPLAY);
                    areaMap->updatePlayScreenMap(alarmId,playedScreen,false);
                }
                CameroNet *cameroNet = CameroNet::getInstance();
                QString fileDir = G_VedioSaveAdd + "/" + alarmMessingMap.value(alarmId).name;
                QDir *dir = new QDir();
                if(!dir->exists(fileDir))
                {
                   dir->mkdir(fileDir);
                }
                QString file = QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss.zzz") + ".mp4";
                QString fileName = fileDir + "/" + file;
                QString *pFileName = new QString(fileName);
                Screen *screen = areaMap->getScreen();
                LLONG PlayHandle = cameroNet->recordVedio(&imf,query.value(1).toInt(),pFileName,screen);
                if(PlayHandle >= 0)//打开成功
                {
                    imf.playId = PlayHandle;
                    areaMap->updatePlayScreenMap(alarmId,screen);
                    QPoint pos = areaMap->getPosByNum(alarmId);
                    screen->setGeometry(pos.x() + 35,pos.y(),280,210);
                    screen->setPlayState(Screen::PLAY);
                    screen->show();
                    //根剧这个判断防区对应的视频是否正在播放或者录像
                    //myTimer *timer = new myTimer(PlayHandle,this);
                    //connect(timer,SIGNAL(timeOut(LONG)),this,SLOT(stopRecord(LONG)));
                    //timer->start(30000);
                    screen->startCountDown(30000);
                    QString file = QString("%1").arg(imf.mold) + fileName;
                    model->setData(model->index(row,VEDIO_PATH),file);

                }
                else {//释放内存

                }
            }
        }

    }
    else if(type == "远程布防" || type == "布防" || type == "自动布防")
    {
        model->setData(model->index(row,ALARM_CLASS),ALARMING_WARMING);
        //z1801->getPartAreaNum(PartType::BYPASS_MESSING,partId);
        //z1801->getPartAreaNum(PartType::FAULT_MESSING,partId);
        //更新左边设备树的状态
        QList<QModelIndex> list = devModel->indexFromPart(partId);
        QModelIndex index;
        foreach (index, list) {
            devModel->setData(index,Node::Defence,Qt::UserRole+1);
        }
        devPanel->update();
    }
    else if (type == "远程撤防" || type == "撤防" || type == "自动撤防") {
        //要获取这个分区在撤防状态下仍布防的防区，比如24小时防区
        //z1801->getPartAreaNum(PartType::DEFENCE_MESSING,partId);
        //z1801->getPartAreaNum(PartType::BYPASS_MESSING,partId);
        //z1801->getPartAreaNum(PartType::FAULT_MESSING,partId);
        model->setData(model->index(row,ALARM_CLASS),ALARMING_WARMING);
        //更新左边设备树的状态
        QList<QModelIndex> list = devModel->indexFromPart(partId);
        QModelIndex index;
        foreach (index, list) {
            devModel->setData(index,Node::OutDefence,Qt::UserRole+1);
        }
        devPanel->update();
    }
    else {
        model->setData(model->index(row,ALARM_CLASS),DEVICE_SYSTEM);
    }
    model->submitAll();
    model->setFilter("IS_HANDLE != 1 OR IS_HANDLE IS null");
    model->select();
    imforView->verticalScrollBar()->setSliderPosition(0);
}

void AlarmWidget::updatePartImfor(const PartImf &imf)
{
    PartImf partImf = imf;
    int partN = partImf.num;
    AlarmImf alarmImf;
    int alarmN;
    Node::Statu statu;
    QModelIndex index;
    foreach (alarmImf, partImf.alarmList) {
        alarmN = alarmImf.alarmNum;
        if(partN == 0)
        {
            index = devModel->indexFromAlarm(alarmN);
        }
        else {
            index = devModel->indexFromUserData(partN,alarmN);
        }
        if(index.isValid())
        {
            //qDebug() << "alarmwidget debug - alarmNum : " << devModel->data(index,Qt::UserRole) << endl;
            // 布防 旁路 故障
            switch (alarmImf.alarmStatu) {
            case AlarmStatu::Defence:
            {
                statu = (Node::Statu)devModel->data(index,Qt::UserRole+1).toInt();
                if(statu != Node::ByPass)
                {
                    devModel->setData(index,Node::Defence,Qt::UserRole+1);
                }
                qDebug() << "alarmwidget debug - defence : " << alarmN <<  endl;
            }
                break;
            case AlarmStatu::ByPass:
            {
                statu = Node::ByPass;
                devModel->setData(index,statu,Qt::UserRole+1);
                qDebug() << "alarmwidget debug - bypass : " << alarmN  << endl;
            }
                break;
            case AlarmStatu::FAULT:
            {
                statu = Node::DisEnale;
                devModel->setData(index,statu,Qt::UserRole+1);
                qDebug() << "alarmwidget debug - fault : "<< alarmN  << endl;
            }
                break;
            default:
                break;
            }

        }
    }
    devPanel->update();
}

void AlarmWidget::showBarMessing(const QString &messing)
{
    statusBar->showMessage(messing,1000);
}

void AlarmWidget::stopRecord(LONG handle)
{
    CameroNet *cameroNet = CameroNet::getInstance();
    cameroNet->stopPlay(handle);

}

void AlarmWidget::alarmNumFromView(QModelIndex index)
{
    int row = index.row();
    int num = model->index(row,0).data().toInt();
    /*if(currentAlarmNum == num)
        return;
    else {
        currentAlarmNum = num;
        emit alarmNumCheck(num);
    }*/
    emit alarmNumCheck(num);
}

void AlarmWidget::rulerChanged()
{
    rulers = ruleDialog->getRulers();
    imforView->update();
}

void AlarmWidget::InitAlarmDeviceImf()
{
    alarmDeviceImf.deviceName = "报警主机";
    alarmDeviceImf.deviceStatu = Node::OffLine;
}

void AlarmWidget::deviceLog()
{
    statusBar->showMessage("设备登陆",1000);
    contralPanel->updateState(true);
}

void AlarmWidget::deviceDefence()
{
    QModelIndex index = devPanel->currentIndex();
    if(index.isValid())
    {
        Node::NodeType type = (Node::NodeType)index.data(Qt::UserRole + 2).toInt();
        if(type == Node::Alarm)
        {
            //提示必须选择分区或者设备才能布防
        }
        else if(type == Node::Partition)
        {
            int part = index.data(Qt::UserRole).toInt();
            z1801->defenceDevice(part,true);
        }
        else if(type == Node::Device)
        {
            z1801->defenceDevice(0,true);
        }

    }
}

void AlarmWidget::deviceDisDefence()
{
    QModelIndex index = devPanel->currentIndex();
    if(index.isValid())
    {
        Node::NodeType type = (Node::NodeType)index.data(Qt::UserRole + 2).toInt();
        if(type == Node::Alarm)
        {
            //提示必须选择分区或者设备才能布防
        }
        else if(type == Node::Partition)
        {
            int part = index.data(Qt::UserRole).toInt();
            z1801->defenceDevice(part,false);
        }
        else if(type == Node::Device)
        {
            z1801->defenceDevice(0,false);
        }

    }
}
void AlarmWidget::alarmDviceImf(AlarmStatu* statuArray,int* PartArray)
{
    alarmDeviceImf.partList.clear();
    QMap<int,AlarmImf> alarmImfMap;
    for(int i = 0;i < 192;i++)
    {
        if(statuArray[i] != AlarmStatu::UN_USED)
        {
            QString name = alarmMessingMap.value(i+1).name;
            if(name.isEmpty())
            {
                name = QString("防区%1").arg(i+1);
            }
            else {
                name = QString("防区%1(%2)").arg(i+1).arg(name);
            }
            AlarmImf alarmImf(i+1,statuArray[i],name);
            alarmImfMap[i+1] = alarmImf;
        }
    }
    QList<int>* partAlarmList[8];
    for(int k = 0;k < 8;k++)
    {
        partAlarmList[k] = new QList<int>;
    }
    int debugPart[192];
    for(int j = 0;j < 192;j++)
    {
        int part = PartArray[j];
        debugPart[j] = PartArray[j];
        if(part < 9 && part > 0)
        {
            partAlarmList[part-1]->append(j+1);
        }
    }
    for(int m = 0;m < 8;m++)
    {
        if(!partAlarmList[m]->isEmpty())
        {
            PartImf partImf;
            partImf.num = m + 1;
            if(partMessingMap.value(m+1).name.isEmpty())
            {
                partImf.partName = QString("第%1分区").arg(m+1);
            }
            else {
                partImf.partName = partMessingMap.value(m + 1).name;
            }
            partImf.partStatu = PartStatus::READY;
            int num;
            foreach (num, *partAlarmList[m])
            {
                partImf.alarmList.append(alarmImfMap.value(num));
            }
            alarmDeviceImf.partList.append(partImf);
        }
    }
    ImforParse *dataParse = new ImforParse();
    Node *rootNode = dataParse->parse(alarmDeviceImf,true);
    devModel->resetRootNode(rootNode);
    devPanel->setModel(devModel);
    devPanel->expandToDepth(2);
    devPanel->update();
}

void AlarmWidget::messingFilter(int state)
{
   if(state == 0)//只显示报警信息
   {
        model->setFilter("ALARM_CLASS == 0 AND (IS_HANDLE != 1 OR IS_HANDLE IS null)");
        model->select();
   }
   else if(state == 1)
   {
       model->setFilter("ALARM_CLASS == 1 AND (IS_HANDLE != 1 OR IS_HANDLE IS null)");
       model->select();
   }
   else
   {
       model->setFilter("IS_HANDLE != 1 OR IS_HANDLE IS null");
       model->select();
   }
}
