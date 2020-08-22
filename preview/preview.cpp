#include "preview.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFile>
#include<QDebug>
#include<QTextStream>
#include<QHeaderView>
#include<QSortFilterProxyModel>
#include<QRegExp>
#include<QMessageBox>
#include "serchedit.h"
#include "addandeditedialog.h"

#include "globdata.h"
Preview::Preview(QWidget *parent)
    : QWidget(parent),
      deviceView(new QTreeView()),
      screenBar(new ScreenBar(4)),
      ptzControl(new PTZControl),
      serchEdit(new SerchEdit),
      addAndEditButton(new QPushButton),
      dialog(nullptr)
{
    hk = CameroNet::getInstance();
    deviceView->setFixedWidth(200);
    deviceView->header()->hide();
    serchEdit->setFixedWidth(170);
    /*QFile file("C:/Users/Administrator/Desktop/l110/deviceImfor.json");
    if(!file.open(QIODevice::ReadWrite |QIODevice::Text))
    {
        qDebug() << "打开文件失败:" << file.errorString() << endl;
    }
    QString data = file.readAll();
    mode = new DeviceTreeMode(data,this);
    deviceView->setModel(mode);*/

    mode = new DeviceTreeMode(deviceImf,this);
    treeModle = mode;
    deviceView->setModel(mode);
    deviceView->expandToDepth(0);
    addAndEditButton->setFixedSize(22,22);
    addAndEditButton->setToolTip("编辑设备信息");
    addAndEditButton->setStyleSheet("QPushButton{border-image:url(:/image/addAndEdit); background:transparent;} \
                                    QPushButton:hover{border-image:url(:/image/addAndEdit)} \
                                    QPushButton:pressed{border-image:url(:/image/addAndEdit)} \
                                    QPushButton::disabled{border-image:url(:/image/addAndEdit)}");
    QHBoxLayout *ltHlayou = new QHBoxLayout();
    ltHlayou->setContentsMargins(6,0,0,0);
    ltHlayou->addWidget(addAndEditButton);
    ltHlayou->addWidget(serchEdit);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0,6,0,0);
    vLayout->setSpacing(0);
    vLayout->addLayout(ltHlayou);
    vLayout->addWidget(deviceView);
    //vLayout->addStretch();
    vLayout->addWidget(ptzControl);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addLayout(vLayout);
    hLayout->addWidget(screenBar);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
    connect(deviceView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(DeviceDoublePress(QModelIndex)));
    connect(hk,SIGNAL(signalDeviceStatusChanged(CameraDeviceImf*,Camero::LineState)),mode,SLOT(changeItemState(CameraDeviceImf *,Camero::LineState)));
    connect(ptzControl,SIGNAL(toDerection(Camero::PTZcommond,int)),this,SLOT(ptz(Camero::PTZcommond,int)));
    connect(serchEdit,SIGNAL(serchStr(const QString &)),this,SLOT(setSerchReusl(const QString &)));
    connect(addAndEditButton,SIGNAL(clicked()),this,SLOT(addAndEditButtonClicked()));
    login(deviceImf);
}


void Preview::DeviceDoublePress(const QModelIndex &index)
{
    QStandardItem *item = mode->itemFromIndex(index);
    if(item->hasChildren())//说明点击的是NVR节点，不做任何处理
        return;
    int serial = item->data(Qt::UserRole + 5).toInt();
    int channel = item->data(Qt::UserRole + 6).toInt();
    CameraDeviceImf &info = deviceImf[serial];
    if(info.luserId == -1)
        return;
    else if(info.playId != -1)//说明已经有播放
        return;
    else
    {
        screenBar->setNextSelectScreen();
        Screen *s = screenBar->getScreen();
        if(s->getPlayState() == Screen::PLAY)
        {
           QMessageBox::warning(this,"提醒","重新选择播放窗口");
           return;
        }
        hk->realPlay(&info,channel,*s);
    }

}

void Preview::playCamero(CameraDeviceImf *camero, Camero::LineState state)
{
    screenBar->setNextSelectScreen();
    Screen *s = screenBar->getScreen();
    if(s->getPlayState() == Screen::PLAY)
    {
        return;
    }
    hk->realPlay(camero,1,*s);
}

void Preview::ptz(Camero::PTZcommond c,int start)
{
    Screen *screen = screenBar->getSelectedScreen();
    CameraDeviceImf *imfo = screen->getCameraDeviceImf();
    hk->PtzControl(*imfo,c,start);
}

void Preview::login(QMap<int, CameraDeviceImf> &imf)
{
    auto map_it = imf.begin();
    while (map_it != imf.end())
    {
        int serial = map_it.key();
        CameraDeviceImf &info = imf[serial];
        hk->login(info);
        map_it++;
    }
}

void Preview::setSerchReusl(const QString &text)
{
    QStandardItem *item = mode->findItem(text);
    if(item == nullptr)
        return;
    QModelIndex index = mode->indexFromItem(item);
    deviceView->setCurrentIndex(index);
}

void Preview::addAndEditButtonClicked()
{
        dialog = new AddAndEditeDialog(this);
        dialog->exec();
}


