#include "rulewidget.h"
#include "ui_rulewidget.h"
#include "voicedelegate.h"
#include "ruleWidget/myedit.h"
#include "customContro/cpushbutton.h"
#include<QPainter>
#include<QPen>
#include<QRectF>
#include<QRect>
#include<QStyleOption>
#include<QHeaderView>
#include<QPushButton>
#include<QModelIndex>
#include<QPalette>
#include<QDebug>
#include<QColorDialog>
#include<QFileInfo>
#include<QSqlRecord>
#include<QByteArray>


enum{
    ZONE_TYPE,
    GROUND_COLOR,
    VOICE,
    VOICE_IS_PLAY,
    PLAY_TIMES,
    ZONE_CODE
};

RuleWidget::RuleWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleWidget),
    soundPlayer(new QSound("")),
    fielDialog(new QFileDialog(this))
{

    ui->setupUi(this);
    path = qApp->applicationDirPath();
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    model.setTable("ruler");
    model.setSort(ZONE_TYPE,Qt::DescendingOrder);
    model.setHeaderData(ZONE_TYPE,Qt::Horizontal,"警情类型");
    model.setHeaderData(GROUND_COLOR,Qt::Horizontal,"背景颜色");
    model.setHeaderData(VOICE,Qt::Horizontal,"提示音");
    model.setHeaderData(VOICE_IS_PLAY,Qt::Horizontal,"播放");
    model.setHeaderData(PLAY_TIMES,Qt::Horizontal,"次数");
    model.select();
    ui->widget->installEventFilter(this);
    ui->tableView->installEventFilter(this);
    ui->tableView->setFixedWidth(407);
    ui->tableView->setModel(&model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnWidth(1,60);
    ui->tableView->setColumnWidth(3,40);
    ui->tableView->setColumnWidth(4,40);
    ui->tableView->setColumnHidden(ZONE_CODE,true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int w = ui->tableView->width();
    for(int i = 0;i< ui->tableView->colorCount();i++)
    {
        if(i == VOICE)
            continue;
        else
          w = w - ui->tableView->columnWidth(i);
    }
    ui->tableView->setColumnWidth(VOICE,w);
    ui->tableView->verticalHeader()->setDefaultSectionSize(20);
    ui->tableView->setAttribute(Qt::WA_Hover);
    ui->tableView->setItemDelegate(new VoiceDelegate(VOICE));
    ui->tableView->setMouseTracking(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(tableClicked(QModelIndex)));
    ui->alarmTypeEdit->setEnabled(false);
    ui->groundColorEdit->setAutoFillBackground(true);
    ui->groundColorEdit->setFocusPolicy(Qt::NoFocus);
    //ui->groundColorEdit->setText("双击设置颜色");
    connect(ui->groundColorEdit,SIGNAL(doubleClicked()),this,SLOT(colorEditClicked()));
    tableClicked(model.index(0,0));
    currentIndex = model.index(0,0);
    ui->addressEdit->setFocusPolicy(Qt::NoFocus);
    ui->colseButton->setButtonStyle(":/image/toolbar_close.png",4);
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(playSound()));
    connect(ui->fileButton,SIGNAL(clicked(bool)),this,SLOT(soundFileSelect()));
    connect(ui->colseButton,SIGNAL(clicked(bool)),this,SLOT(closeButtonClicked()));
    connect(ui->applyButton,SIGNAL(clicked(bool)),this,SLOT(applyButtonClicked()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelButtonClicked()));
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(okButtonClicked()));
}

RuleWidget::~RuleWidget()
{
    delete ui;
    if(soundPlayer)
    {
        soundPlayer->stop();
        delete soundPlayer;
    }
}

void RuleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setPen(QPen(Qt::black,lineWidth));
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(1,1,width()-lineWidth,height()-lineWidth,0,0);
}



bool RuleWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget)
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
        }
    }
    else if(obj == ui->tableView)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->type() == QMouseEvent::MouseButtonDblClick)
        {

            event->ignore();
            return true;
        }
    }
    return false;
}

void RuleWidget::tableClicked(const QModelIndex &index)
{
    int row = index.row();
    currentIndex = index;
    ui->alarmTypeEdit->setText(model.index(row,0).data().toString());
    QByteArray d = "ff" + model.index(row,1).data().toByteArray().remove(0,1);
    QRgb c = d.toUInt(nullptr,16);
    QPalette pa(ui->groundColorEdit->palette());
    pa.setColor(QPalette::Base,QColor(c));
    ui->groundColorEdit->setPalette(pa);
    QString str = model.index(row,3).data().toString();
    if(str == "是")
    {
        ui->checkBox->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox->setCheckState(Qt::Unchecked);
    }
    ui->addressEdit->setText(model.index(row,2).data().toString());
    ui->spinBox->setValue(model.index(row,4).data().toInt());

}

void RuleWidget::playSound()
{
    if(soundPlayer == nullptr)
        return;
    soundPlayer->stop();
    delete soundPlayer;
    int n = ui->spinBox->value();
    QString str = ui->addressEdit->text();
    QString adr = path + "/voice/" + str;
    soundPlayer = new QSound(adr);
    soundPlayer->setLoops(n);
    soundPlayer->play();
}

void RuleWidget::colorEditClicked()
{
    QColor c = QColorDialog::getColor();
    QPalette pa(ui->groundColorEdit->palette());
    pa.setColor(QPalette::Base,c);
    ui->groundColorEdit->setPalette(pa);
}

void RuleWidget::soundFileSelect()
{
    QString str = QFileDialog::getOpenFileName(this,"声音文件","./debug/voice/","sound files(*.wav)");
    if(str.isEmpty())
        return;
    QFileInfo info(str);
    QString fileName = info.fileName();
    ui->addressEdit->setText(fileName);
}

void RuleWidget::applyButtonClicked()
{
    QRgb c = ui->groundColorEdit->palette().color(QPalette::Base).rgb();
    QByteArray d = "#" + QByteArray::number(c,16).remove(0,2);
    QString zoneType = ui->alarmTypeEdit->text();
    QString filter = QString("ZONE_TYPE = '%1'").arg(zoneType);
    model.setFilter(filter);
    model.select();
    QSqlRecord record = model.record(0);
    record.setValue("GROUND_COLOR",d);
    record.setValue("VIOCE",ui->addressEdit->text());
    record.setValue("VIOCE_IS_PLAY",ui->checkBox->isChecked() ? "是" : "否");
    record.setValue("PLAY_TIMES",ui->spinBox->value());
    model.setRecord(0,record);
    model.setFilter("");
    model.select();
    model.submitAll();
    emit rulerChange();
}

void RuleWidget::closeButtonClicked()
{
    this->close();
    if(soundPlayer)
    {
        soundPlayer->stop();
    }
}

QMap<QString,Ruler> RuleWidget::getRulers()
{
    model.setFilter("");
    model.select();
    for(int i = 0;i< model.rowCount();i++)
    {
        QSqlRecord record = model.record(i);
        QString rulerType = record.value("ZONE_TYPE").toString();
        QByteArray d = "ff" + record.value("GROUND_COLOR").toByteArray().remove(0,1);
        QRgb rgb = d.toUInt(nullptr,16);
        QString soundAddress = path + "/voice/" + record.value("VIOCE").toString();
        bool isplay = (record.value("VIOCE_IS_PLAY").toString() == "是") ? true : false;
        int playTimes = record.value("PLAY_TIMES").toInt();
        Ruler ruler;
        ruler.groundColor = rgb;
        ruler.soundAddress = soundAddress;
        ruler.isPlay = isplay;
        ruler.playTimes= playTimes;
        rulers.insert(rulerType,ruler);
    }
    return rulers;
}

void RuleWidget::cancelButtonClicked()
{

}

void RuleWidget::okButtonClicked()
{

}
