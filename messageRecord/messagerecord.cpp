#include "messagerecord.h"
#include "alarmwidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlQueryModel>
#include <QScrollBar>
#include "viewitemdelege.h"
#include "player.h"
#include "bottomstatu.h"

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

MessageRecord::MessageRecord(QWidget *parent)
    :QWidget(parent),
      submit(new SubmitWidget()),
      messageView(new MessageView),
      bottomWidget(new BottomStatu(this)),
      player(new Player(this))
{
    player->hide();
    RuleWidget rw;
    rulers = rw.getRulers();
    model = new CSqlQueryModel(&rulers,this);
    messageView->view->setModel(model);
    model->setQuery("SELECT * FROM alarm WHERE REPORT_TIME < 2000");
    model->setHeaderData(ZONE_NUM,Qt::Horizontal,"防区编号");
    model->setHeaderData(ZONE_NAME,Qt::Horizontal,"防区名称");
    model->setHeaderData(ZONE_TYPE,Qt::Horizontal,"防区类型");
    model->setHeaderData(EVENT_TIME,Qt::Horizontal,"事件时间");
    model->setHeaderData(REPORT_TIME,Qt::Horizontal,"上报时间");
    model->setHeaderData(PART_NUM,Qt::Horizontal,"分区编号");
    model->setHeaderData(PART_NAME,Qt::Horizontal,"分区名称");
    model->setHeaderData(CHARG1_NAME,Qt::Horizontal,"第一联系人");
    model->setHeaderData(CHARG1_PHONE,Qt::Horizontal,"联系人电话");
    model->setHeaderData(CHARG2_NAME,Qt::Horizontal,"第二联系人");
    model->setHeaderData(CHARG2_PHONE,Qt::Horizontal,"联系人电话");
    model->setHeaderData(IS_HANDLE,Qt::Horizontal,"已处理");
    model->setHeaderData(HANDLE_PERSON,Qt::Horizontal,"平台处理人");
    model->setHeaderData(HANDLE_OPINION,Qt::Horizontal,"警情意见");
    model->setHeaderData(NOTICE_PERSON,Qt::Horizontal,"现场处理人");
    model->setHeaderData(NOTICE_PHONE,Qt::Horizontal,"处理人电话");
    model->setHeaderData(VEDIO_PATH,Qt::Horizontal,"录像");
    messageView->view->setColumnHidden(ALARM_CLASS,true);
    messageView->view->setColumnHidden(CHARG1_NAME,true);
    messageView->view->setColumnHidden(CHARG1_PHONE,true);
    messageView->view->setColumnHidden(CHARG2_NAME,true);
    messageView->view->setColumnHidden(CHARG2_PHONE,true);
    messageView->view->setColumnHidden(MAIN_ALARM_KEY,true);
    ViewItemDelege *delege = new ViewItemDelege(this);
    connect(delege,SIGNAL(play(QModelIndex)),this,SLOT(itemButtonClicked(QModelIndex)));
    messageView->view->setItemDelegateForColumn(VEDIO_PATH,delege);
    messageView->view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    messageView->view->setSelectionBehavior(QAbstractItemView::SelectRows);
    messageView->view->setColumnWidth(ZONE_NUM,60);
    messageView->view->setColumnWidth(ZONE_NAME,160);
    messageView->view->verticalHeader()->setDefaultSectionSize(40);
    messageView->view->horizontalHeader()->setStretchLastSection(true);
    //messageView->view->horizontalHeader()->swapSections(VEDIO_PATH,IS_HANDLE);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->addWidget(submit);
    vLayout->addWidget(messageView);
    vLayout->addWidget(bottomWidget);
    setLayout(vLayout);
    submit->setModel(model);
    bottomWidget->setModle(model);

    connect(submit,SIGNAL(queryResult(int)),bottomWidget,SLOT(setTotalRecordCount(int)));
    connect(submit,SIGNAL(filterChanged(const QString)),bottomWidget,SLOT(setFilter(const QString)));
    connect(bottomWidget,SIGNAL(pageChanged()),this,SLOT(update()));
}

void MessageRecord::update()
{
    messageView->view->verticalScrollBar()->setSliderPosition(0);
}

void MessageRecord::itemButtonClicked(QModelIndex index)
{
    QString str = index.data().toString();
    QString moldStr = str.mid(0,1);
    int mold = moldStr.toLatin1().data()[0] - 48;
    str.remove(0,1);
    player->reSet();
    player->hide();
    player->play(str,mold);
    player->raise();
    player->setGeometry(450,200,460,345);
    player->show();
}

void MessageRecord::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    this->setAttribute(Qt::WA_Mapped);
}
