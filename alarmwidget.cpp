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
#include <QThread>
#include"globdata.h"
#include"z1801p.h"
#include "bombScreen/bombscreen.h"
#include "globaloption.h"
#include "mytimer.h"
#include "ks500a.h"
#include "globdata.h"
#include "devicemodel/icondeswidget.h"
#include "log4qt/logger.h"



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
    VEDIO_PATH,
    MAIN_ALARM_KEY
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
      soundPlayer(new SoundPlayer(this)),
      popWidget(new PopWidget(this))
{
    //updateNickNmae(glob_user);
    InitAlarmDeviceImf();
    if(mType == MachineType::KS_500A)
    {
        updateDefenceAlarmNumList();
    }
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    statusBar->setFixedWidth(200);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    IconDesWidget *iconDes= new IconDesWidget(this);
    leftLayout->setContentsMargins(0,2,0,0);
    leftLayout->setSpacing(10);
    leftLayout->addWidget(contralPanel);
    leftLayout->addWidget(devPanel);
    leftLayout->addWidget(iconDes);
    //leftLayout->addStretch();
    statusBar->hide();
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
    popWidget->setModel(model);
    /*proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(ALARM_CLASS);*/
    imforView->installEventFilter(this);
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
    imforView->setColumnHidden(MAIN_ALARM_KEY,true);
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
    delete dataParse;
    devModel = new DeviceModel(this);
    devModel->setRootNode(rootNode);
    devPanel->setModel(devModel);
    devPanel->setItemDelegate(new NodeDelegate());
    devPanel->setFixedWidth(200);
    devPanel->header()->hide();
    devPanel->setObjectName("alarmTree");
    devPanel->expandToDepth(2);
    connect(z1801,SIGNAL(getDeviceImfFinished(AlarmStatu *,int *)),this,SLOT(alarmDviceImf(AlarmStatu*,int*)));
    connect(z1801,SIGNAL(haveAlarmMessing(AlarmMessing)),this,SLOT(alarmMessing(AlarmMessing)));
    connect(z1801,SIGNAL(deviceLogin()),this,SLOT(deviceLog()));
    connect(z1801,SIGNAL(updatePartImfor(const PartImf &)),this,SLOT(updatePartImfor(const PartImf &)));
    connect(z1801,SIGNAL(barMessing(const QString &)),this,SLOT(showBarMessing(const QString &)));
    connect(z1801,&Z1801::deviceOff,this,&AlarmWidget::alarmDviceOff);
    connect(ruleDialog,SIGNAL(rulerChange()),this,SLOT(rulerChanged()));
    connect(contralPanel,SIGNAL(setDevice()),deviceSetWidget,SLOT(show()));
    connect(contralPanel,SIGNAL(defence()),this,SLOT(deviceDefence()));
    connect(contralPanel,SIGNAL(disDefence()),this,SLOT(deviceDisDefence()));
    connect(devPanel,&QTreeView::doubleClicked,this,&AlarmWidget::alarmNumFromTree);
    connect(imforView,SIGNAL(clicked(QModelIndex)),this,SLOT(alarmNumFromView(QModelIndex)));
    connect(this,SIGNAL(alarmNumCheck(int)),areaMap,SLOT(alarmNumChecked(int)));
    connect(controWidget,SIGNAL(stateChange(int)),this,SLOT(messingFilter(int)));
    connect(controWidget,SIGNAL(handleHappened()),areaMap,SLOT(closeChannal()));
    connect(dataEntryDialog,&DataEntryDialog::deleNumList,areaMap,&AreaMap::deleOptionbyNumList);
    connect(dataEntryDialog,&DataEntryDialog::dataChange,this,&AlarmWidget::dataChanged);
    connect(Ks500a::get500a(),&Ks500a::messing,this,&AlarmWidget::hand500Amessing);
    connect(contralPanel,&ContralPanel::serchTest,this,&AlarmWidget::findStr);
    //BombScreen *screen = new BombScreen(,this);
}

void AlarmWidget::showEvent(QShowEvent *event)
{

}


bool AlarmWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == imforView)
    {
        if(event->type() == QEvent::FocusOut)
        {
            event->ignore();
            return true;
        }
    }
    return false;
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

void AlarmWidget::findStr(const QString str)
{
   QModelIndex index = devModel->findText(str);
   devPanel->setCurrentIndex(index);
   devPanel->setFocus();
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

void AlarmWidget::hand500Amessing(AlarmMessing &messing)
{
    if(messing.aLarmID == 0)
    {
        messing.zoneId = 0;
    }
    else
    {
        QSqlQuery query;
        QString sql = QString("SELECT PART_NUM FROM alarmImfor WHERE ALARM_NUM = %1")
                .arg(messing.aLarmID);
        query.exec(sql);
        if(query.next())
        {
            messing.zoneId = query.value(0).toInt();
        }
        else
        {
            messing.zoneId = 0;
        }
    }
    alarmMessing(messing);
}

void AlarmWidget::alarmMessing(AlarmMessing messing)
{
    //if(messing.type != AlarmType::STONE_ALARM)
    //return;
    int row = 0;
    int alarmId = messing.aLarmID;
    int partId = messing.zoneId;
    Log4Qt::Logger::logger("Z1801")->debug(QString("%1").arg(messing.type));
    QString type = messingTypeMap.value(messing.type);
    if(type.isEmpty())
        type = messing.typeDes;
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

    model->submit();
    //播放绑定的视频
    if((mType==MachineType::KS_Z801A && (type == "24小时有声报警" || type == "出/入口报警" || type == "24小时无声报警" || type == "防盗防区"
            || type == "火警防区" || type == "紧急报警")) || (mType == MachineType::KS_500A &&(type == "防盗报警" || type == "防拆报警")))
    {
        //areaMap->alarmNumChecked(alarmId);

        //左下角弹出报警框
        //传一个index和model的指针
        int main_alarm_key = model->index(0,MAIN_ALARM_KEY).data().toInt();
        popWidget->addIndex(main_alarm_key);
        popWidget->show();

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
                    Log4Qt::Logger::logger("AlarmWidget")->debug(QString("开启录像成功，句柄为%1").arg(PlayHandle));
                    imf.playId = PlayHandle;
                    areaMap->updatePlayScreenMap(alarmId,screen);
                    QPoint pos = areaMap->getPosByNum(alarmId);
                    screen->setGeometry(pos.x()+15,pos.y() - 34,280,210);
                    screen->setPlayState(Screen::PLAY);
                    screen->show();
                    //根剧这个判断防区对应的视频是否正在播放或者录像
                    //myTimer *timer = new myTimer(PlayHandle,this);
                    //connect(timer,SIGNAL(timeOut(LONG)),this,SLOT(stopRecord(LONG)));
                    //timer->start(30000);
                    screen->startCountDown(G_RecordTime * 1000);
                    QString file = QString("%1").arg(imf.mold) + fileName;
                    model->setData(model->index(row,VEDIO_PATH),file);

                }
                else {//释放内存

                }
            }
        }
        setCursor(Qt::ArrowCursor);
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
    else if(mType == MachineType::KS_500A && (type == "主机布防"))
    {
        QList<QModelIndex> list = devModel->indexFromPart(0);
        QModelIndex index;
        foreach (index, list) {
            devModel->setData(index,Node::Defence,Qt::UserRole+1);
        }
        alarmDeviceImf.deviceStatu = Node::Defence;
        devPanel->update();

    }
    else if(mType == MachineType::KS_500A && (type == "主机撤防"))
    {
        QList<QModelIndex> list = devModel->indexFromPart(0);
        QModelIndex index;
        foreach (index, list) {
            if(defenceAlarmNumList.contains(index.data(Qt::UserRole).toInt()))
            {
               devModel->setData(index,Node::Defence,Qt::UserRole+1);
            }
            else
            {
                devModel->setData(index,Node::OutDefence,Qt::UserRole+1);
            }
        }
        alarmDeviceImf.deviceStatu = Node::OutDefence;
        devPanel->update();
    }
    else if(type == "设备上线")
    {
        QModelIndex index = devModel->getDeviceIndex();
        devModel->setData(index,Node::Online,Qt::UserRole+1);
        devPanel->update();
    }
    else if(type == "设备下线" || type == "设备离线")
        {
        QModelIndex index = devModel->getDeviceIndex();
        devModel->setData(index,Node::OffLine,Qt::UserRole+1);
        QList<QModelIndex> list = devModel->indexFromPart(0);
        QModelIndex idx;
        foreach (idx, list) {
            devModel->setData(idx,Node::NoStaru,Qt::UserRole+1);
        }
        alarmDeviceImf.deviceStatu = Node::OffLine;
        devPanel->update();
    }
    else {
        model->setData(model->index(row,ALARM_CLASS),DEVICE_SYSTEM);
    }
    //播放声音
    Ruler ruler = rulers.value(type);
    soundPlayer->stopPlay();
    soundPlayer->playSound(ruler.soundAddress,ruler.playTimes);
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

void AlarmWidget::alarmNumFromTree(QModelIndex index)
{
    if(index.data(Qt::UserRole + 2)==Node::Alarm)
    {
        int num = index.data(Qt::UserRole).toInt();
        emit alarmNumCheck(num);
    }
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

void AlarmWidget::updateAlarmDeviceImf()
{

        QSqlQuery query;
        QString sql = "SELECT PART_NUM,PART_NAME FROM partImfor";
        query.exec(sql);
        PartImf partImf;
        AlarmImf alarmImf;
        alarmDeviceImf.partList.clear();
        while(query.next())
        {
            partImf.num = query.value(0).toInt();
            partImf.partName = query.value(1).toString();
            if(partImf.partName.isEmpty())
            {
                partImf.partName = QString("第%1分区").arg(partImf.num);
            }
            alarmDeviceImf.partList.append(partImf);
        }
        if(alarmDeviceImf.partList.length() == 0)
        {
            partImf.num = -1;
            partImf.partName = "";
            alarmDeviceImf.partList.append(partImf);
        }
        for(auto item = alarmDeviceImf.partList.begin();
            item != alarmDeviceImf.partList.end();
            item++)
        {
            int partNum = item->num;
            if(partNum != -1)
            {
                sql = QString("SELECT ALARM_NUM,ALARM_NAME FROM alarmImfor WHERE PART_NUM = %1").arg(partNum);
            }
            else
            {
                sql = QString("SELECT ALARM_NUM,ALARM_NAME FROM alarmImfor");
            }
            query.exec(sql);
            while (query.next()) {
                alarmImf.alarmNum = query.value(0).toInt();
                alarmImf.alarmName = query.value(1).toString();
                if(alarmImf.alarmName.isEmpty())
                {
                    alarmImf.alarmName = QString("第%1防区").arg(alarmImf.alarmNum);
                }
                //alarmImf.alarmStatu = AlarmStatu::OutDefence;
                item->alarmList.append(alarmImf);
            }
        }

}

void AlarmWidget::updateDefenceAlarmNumList()
{
    defenceAlarmNumList.clear();
    QSqlQuery query;
    QString sql = "SELECT alarm_num FROM alarmImfor WHERE ALARM_TYPE"
                  " IN (0x1122,0x1123,0x1110,0x1151)";
    query.exec(sql);
    while(query.next()){
        defenceAlarmNumList << query.value(0).toInt();
    }
}

void AlarmWidget::InitAlarmDeviceImf()
{
    QSqlQuery query;
    QString sql = "SELECT ALARM_DEVICE_NAME FROM alarmDeviceImf";
    query.exec(sql);
    QString name;
    if(query.next())
    {
        name = query.value(0).toString();
    }
    else
    {
        name = "报警主机";
    }
    alarmDeviceImf.deviceName = name;
    alarmDeviceImf.deviceStatu = Node::OffLine;
    if(mType == MachineType::KS_500A)
    {
        sql = "SELECT PART_NUM,PART_NAME FROM partImfor";
        query.exec(sql);
        PartImf partImf;
        AlarmImf alarmImf;
        while(query.next())
        {
            partImf.num = query.value(0).toInt();
            partImf.partName = query.value(1).toString();
            if(partImf.partName.isEmpty())
            {
                partImf.partName = QString("第%1分区").arg(partImf.num);
            }
            alarmDeviceImf.partList.append(partImf);
        }
        if(alarmDeviceImf.partList.length() == 0)
        {
            partImf.num = -1;
            partImf.partName = "";
            alarmDeviceImf.partList.append(partImf);
        }
        for(auto item = alarmDeviceImf.partList.begin();
            item != alarmDeviceImf.partList.end();
            item++)
        {
            int partNum = item->num;
            if(partNum != -1)
            {
                sql = QString("SELECT ALARM_NUM,ALARM_NAME FROM alarmImfor WHERE PART_NUM = %1").arg(partNum);
            }
            else
            {
                sql = QString("SELECT ALARM_NUM,ALARM_NAME FROM alarmImfor");
            }
            query.exec(sql);
            while (query.next()) {
                alarmImf.alarmNum = query.value(0).toInt();
                alarmImf.alarmName = query.value(1).toString();
                if(alarmImf.alarmName.isEmpty())
                {
                    alarmImf.alarmName = QString("第%1防区").arg(alarmImf.alarmNum);
                }
                //alarmImf.alarmStatu = AlarmStatu::OutDefence;
                item->alarmList.append(alarmImf);
            }
        }
    }
}

void AlarmWidget::dataChanged()
{
    updateDefenceAlarmNumList();
    updateAlarmDeviceImf();
    QSqlQuery query;
    query.exec("SELECT ALARM_NUM,ALARM_NAME FROM alarmImfor");
    while(query.next())
    {
        int num = query.value(0).toInt();
        QString name = query.value(1).toString();
        nameMap[num] = name;
    }
    ImforParse *dataParse = new ImforParse();
    Node *rootNode = dataParse->parse(alarmDeviceImf,false);
    delete dataParse;
    devModel->resetRootNode(rootNode);
    devPanel->setModel(devModel);
    devPanel->expandToDepth(2);
    if(alarmDeviceImf.deviceStatu == Node::Defence)
    {
        QList<QModelIndex> list = devModel->indexFromPart(0);
        QModelIndex index;
        foreach (index, list) {
            devModel->setData(index,Node::Defence,Qt::UserRole+1);
        }
        index = devModel->getDeviceIndex();
        devModel->setData(index,Node::Online,Qt::UserRole+1);
    }
    else if(alarmDeviceImf.deviceStatu == Node::OutDefence)
    {
        QList<QModelIndex> list = devModel->indexFromPart(0);
        QModelIndex index;
        foreach (index, list) {
            if(defenceAlarmNumList.contains(index.data(Qt::UserRole).toInt()))
            {
               devModel->setData(index,Node::Defence,Qt::UserRole+1);
            }
            else
            {
                devModel->setData(index,Node::OutDefence,Qt::UserRole+1);
            }
        }
        index = devModel->getDeviceIndex();
        devModel->setData(index,Node::Online,Qt::UserRole+1);
    }
    devPanel->update();
}

void AlarmWidget::deviceLog()
{
    statusBar->showMessage("设备登陆",1000);
    contralPanel->updateState(true);
}

void AlarmWidget::deviceDefence()
{
    if(z1801 == nullptr)
        return;
    if(!z1801->isLogin())
        return;
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
    if(z1801 == nullptr)
        return;
    if(!z1801->isLogin())
        return;
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

void AlarmWidget::alarmDviceOff()
{
    alarmDeviceImf.partList.clear();
    InitAlarmDeviceImf();
    ImforParse *dataParse = new ImforParse();
    Node *rootNode = dataParse->parse(alarmDeviceImf,false);
    devModel->resetRootNode(rootNode);
    devPanel->setModel(devModel);
    devPanel->expandToDepth(2);
    devPanel->update();
    delete dataParse;
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
    delete  dataParse;
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
