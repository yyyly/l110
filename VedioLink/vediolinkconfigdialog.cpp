#include "vediolinkconfigdialog.h"
#include "ui_vediolinkconfigdialog.h"
#include "globdata.h"
#include <QStringListModel>
#include <QSqlQuery>
#include <QModelIndex>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QPushButton>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>


VedioLinkConfigDialog::VedioLinkConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VedioLinkConfigDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->widget->installEventFilter(this);
    ui->closePushButton->setButtonStyle(":/image/toolbar_close.png",4);
    setFixedSize(800,450);
    w = new CTableWidget(this);
    QStringListModel modle;
    QList<int> alarmNumList;
    QSqlQuery query;
    query.exec("SELECT ALARM_NUM FROM alarmImfor");
    while(query.next())
    {
        int n = query.value(0).toInt();
        alarmNumList.append(n);
    }

    alarmModle = new IntListModle(alarmNumList,nameMap,"防区",this);
    alarmModle->setHeaderData(0,Qt::Horizontal,"防区列表");
    ui->alarmTableView->setModel(alarmModle);

    nvrModel = new QSqlTableModel(this);
    nvrModel->setTable("deviceImform");
    nvrModel->setHeaderData(2,Qt::Horizontal,"视频主机");
    nvrModel->select();
    ui->deletePushButton->setToolTip("解除绑定");
    ui->addPushButton->setToolTip("添加绑定");
    ui->nvrTableView->setModel(nvrModel);
    ui->nvrTableView->setColumnHidden(0,true);
    ui->nvrTableView->setColumnHidden(1,true);
    ui->nvrTableView->setColumnHidden(3,true);
    ui->nvrTableView->setColumnHidden(4,true);
    ui->nvrTableView->setColumnHidden(5,true);
    ui->nvrTableView->setColumnHidden(6,true);
    ui->nvrTableView->setColumnHidden(7,true);
    ui->nvrTableView->setColumnHidden(8,true);
    ui->nvrTableView->setColumnHidden(9,true);
    ui->nvrTableView->horizontalHeader()->setStretchLastSection(true);
    ui->nvrTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->nvrTableView->setCurrentIndex(nvrModel->index(0,0));
    ui->chanleListView->horizontalHeader()->setStretchLastSection(true);
    ui->nvrTableView->verticalHeader()->setVisible(false);
    ui->chanleListView->verticalHeader()->setVisible(false);
    ui->alarmTableView->horizontalHeader()->setStretchLastSection(true);
    updateChannel();
    connect(ui->nvrTableView->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &))
            ,this,SLOT(updateChannel()));

    linkModle = new QSqlTableModel(this);
    linkModle->setTable("vedioLink");
    linkModle->setHeaderData(2,Qt::Horizontal,"视频主机");
    linkModle->setHeaderData(4,Qt::Horizontal,"通道号");
    linkModle->select();
    ui->LinkTableView->setModel(linkModle);
    ui->LinkTableView->setColumnHidden(0,true);
    ui->LinkTableView->setColumnHidden(1,true);
    ui->LinkTableView->setColumnHidden(3,true);
    ui->LinkTableView->horizontalHeader()->setStretchLastSection(true);
    ui->LinkTableView->setCurrentIndex(linkModle->index(0,0));
    ui->LinkTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->LinkTableView->verticalHeader()->setVisible(false);
    connect(ui->alarmTableView->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &))
            ,this,SLOT(updateLinkView()));
}

VedioLinkConfigDialog::~VedioLinkConfigDialog()
{
    delete ui;
}

bool VedioLinkConfigDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget && !this->isMaximized())
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        switch(mouseEvent->type())
        {
        case QEvent::MouseButtonPress:
            {
                if(mouseEvent->button() == Qt::LeftButton)
                {
                    mouseIsPress = true;
                    mousePos = mouseEvent->pos();
                }
            }
                break;
        case QEvent::MouseMove:
            {
                move(mouseEvent->pos() - mousePos  + pos());
            }
                break;
        case QEvent::MouseButtonRelease:
            {
                mouseIsPress = false;
            }
                break;
        default:
            break;
        }
    }
    return false;
}

void VedioLinkConfigDialog::updateChannel()
{
    QModelIndex index = ui->nvrTableView->currentIndex();
    if(index.isValid())
    {
        chanmelNames.clear();
        QList<int> numList;
        QSqlRecord record = nvrModel->record(index.row());
        int serial = record.value("serial").toInt();
        int amount = record.value("channel_amount").toInt();
        QSqlQuery query;
        query.exec(QString("SELECT channel_number,channel_name FROM channelName WHERE serial == %1").arg(serial));
        while(query.next())
        {
            int num = query.value(0).toInt();
            QString name = query.value(1).toString();
            chanmelNames[num] = name;
        }
        for(int i = 1;i <= amount;i++)
        {
            numList << i;
        }
        IntListModle *model =  new IntListModle(numList,chanmelNames,"通道",this);
        ui->chanleListView->setModel(model);


    }
}

void VedioLinkConfigDialog::updateLinkView()
{
    QModelIndex index = ui->alarmTableView->currentIndex();
    if(index.isValid())
    {
        int alarmNum = index.data(Qt::EditRole).toInt();
        linkModle->setFilter(QString("ALARM_NUM = %1").arg(alarmNum));
        linkModle->select();
    }
}

void VedioLinkConfigDialog::setItemWidget()
{
}



void VedioLinkConfigDialog::on_addPushButton_clicked()
{
    int alarmNum = ui->alarmTableView->currentIndex().data(Qt::EditRole).toInt();
    int row = ui->nvrTableView->currentIndex().row();
    int serial  = nvrModel->record(row).value("serial").toInt();
    int channelAmount = nvrModel->record(row).value("channel_amount").toInt();
    QString deviceName = nvrModel->record(row).value("name").toString();
    int channal = ui->chanleListView->currentIndex().data(Qt::EditRole).toInt();
    QString channalName = ui->chanleListView->currentIndex().data(Qt::DisplayRole).toString();
    if(channalName.isEmpty())
    {
        if(channelAmount > 1)
        {
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setText("录像机未选中视频通道");
            box.setStandardButtons(QMessageBox::Ok);
            int ret = box.exec();
            if(ret == QMessageBox::Ok)
            {
                return;
            }
        }
        else
        {
            channal = 1;
            channalName = "通道1";
        }

    }
    QSqlQuery query;
    query.exec(QString("SELECT serial,channal_num FROM vedioLink WHERE ALARM_NUM = %1").arg(alarmNum));
    while (query.next()) {
        if(query.value(0) == serial && query.value(1) == channal)
        {
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setText("选中防区已绑定此通道");
            box.setStandardButtons(QMessageBox::Ok);
            int ret = box.exec();
            if(ret == QMessageBox::Ok)
            {
                return;
            }
        }
        }
    if(linkModle->rowCount() == 1)
    {
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setText("此版本暂只支持绑定一路通道");
        box.setStandardButtons(QMessageBox::Ok);
        int ret = box.exec();
        if(ret == QMessageBox::Ok)
        {
            return;
        }
    }
    linkModle->insertRow(0);
    linkModle->setData(linkModle->index(0,0),alarmNum);
    linkModle->setData(linkModle->index(0,1),serial);
    linkModle->setData(linkModle->index(0,2),deviceName);
    linkModle->setData(linkModle->index(0,3),channal);
    linkModle->setData(linkModle->index(0,4),channalName);
    linkModle->submit();
}

void VedioLinkConfigDialog::on_deletePushButton_clicked()
{
    int alarmNum = ui->alarmTableView->currentIndex().data(Qt::EditRole).toInt();
    QSqlQuery query;
    query.exec(QString("DELETE  FROM vedioLink WHERE ALARM_NUM = %1").arg(alarmNum));
    updateLinkView();
}

void VedioLinkConfigDialog::on_closePushButton_clicked()
{
    reject();
    close();
}

void VedioLinkConfigDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setPen(QPen(Qt::black,2));
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(1,1,width()-2,height()-2,0,0);
}
