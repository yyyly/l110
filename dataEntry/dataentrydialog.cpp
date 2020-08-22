#include "dataentrydialog.h"
#include "ui_dataentrydialog.h"
#include <QModelIndexList>
#include <QSqlQuery>
#include <QMessageBox>
#include <QScopedPointer>
#include <QDebug>
#include <QPainter>
#include "style/customtabwidgetstyle.h"
#include "globdata.h"

#define min(a,b) ((a)<(b)? (a) :(b))
#define max(a,b) ((a)>(b)? (a) :(b))

DataEntryDialog::DataEntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataEntryDialog)

{
    ui->setupUi(this);
    this->setMinimumSize(972,540);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    deviceModel.setTable("alarmDeviceImf");
    deviceModel.select();
    mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper.setModel(&deviceModel);
    mapper.addMapping(ui->alarmDeviceNamelineEdit,0);
    mapper.addMapping(ui->charge1lineEdit,1);
    mapper.addMapping(ui->charg1PhoneEdit,2);
    mapper.addMapping(ui->charge2lineEdit,3);
    mapper.addMapping(ui->charg2PhoneEdit,4);
    mapper.setCurrentIndex(0);
    partModel.setTable("partImfor");
    partModel.setSort(0,Qt::AscendingOrder);
    partModel.setHeaderData(0,Qt::Horizontal,"分区编号",Qt::DisplayRole);
    partModel.setHeaderData(1,Qt::Horizontal,"分区名称",Qt::DisplayRole);
    partModel.setHeaderData(2,Qt::Horizontal,"负责人1姓名",Qt::DisplayRole);
    partModel.setHeaderData(3,Qt::Horizontal,"负责人1电话",Qt::DisplayRole);
    partModel.setHeaderData(4,Qt::Horizontal,"负责人2姓名",Qt::DisplayRole);
    partModel.setHeaderData(5,Qt::Horizontal,"负责人2电话",Qt::DisplayRole);
    partModel.select();
    ui->partTableView->setModel(&partModel);
    ui->partTableView->verticalHeader()->setVisible(false);
    ui->partTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->partTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    alarmModel.setTable("alarmImfor");
    alarmModel.setHeaderData(0,Qt::Horizontal,"防区编号",Qt::DisplayRole);
    alarmModel.setHeaderData(1,Qt::Horizontal,"防区名称",Qt::DisplayRole);
    alarmModel.setHeaderData(2,Qt::Horizontal,"负责人1姓名",Qt::DisplayRole);
    alarmModel.setHeaderData(3,Qt::Horizontal,"负责人1电话",Qt::DisplayRole);
    alarmModel.setHeaderData(4,Qt::Horizontal,"负责热2姓名",Qt::DisplayRole);
    alarmModel.setHeaderData(5,Qt::Horizontal,"负责人2电话",Qt::DisplayRole);
    alarmModel.setHeaderData(8,Qt::Horizontal,"防区类型",Qt::DisplayRole);
    alarmModel.setHeaderData(9,Qt::Horizontal,"所属分区",Qt::DisplayRole);
    alarmModel.select();
    ui->alarmTableView->setModel(&alarmModel);
    ui->alarmTableView->verticalHeader()->setVisible(false);
    ui->alarmTableView->setColumnHidden(6,true);
    ui->alarmTableView->setColumnHidden(7,true);
    ui->alarmTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->alarmTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->widget->installEventFilter(this);
    ui->closePushButton->setButtonStyle(":/image/toolbar_close.png",4);
    ui->partTabWidget->setTabPosition(QTabWidget::West);
    ui->partTabWidget->tabBar()->setStyle(new CustomTabWidgetStyle);

    edgeMargin = 4;        //设置检测边缘为4
    resizeDir = nodir;   //初始化检测方向为无
    setMouseTracking(true); //开启鼠标追踪
}

DataEntryDialog::~DataEntryDialog()
{
    delete ui;
}

void DataEntryDialog::mousePressEvent(QMouseEvent * event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
     {
       dragPosition = event->globalPos() - frameGeometry().topLeft();  //获得鼠标按键位置相对窗口左上面的位置

     }
}

void DataEntryDialog::paintEvent(QPaintEvent *event)
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
void DataEntryDialog::testEdge()
{
    int diffLeft = abs(cursor().pos().x() - frameGeometry().left());      //计算鼠标距离窗口上下左右有多少距离
    int diffRight = abs(cursor().pos().x() - frameGeometry().right());
    int diffTop = abs(cursor().pos().y() - frameGeometry().top());
    int diffBottom = abs(cursor().pos().y() - frameGeometry().bottom());

    QCursor tempCursor;                                    //获得当前鼠标样式，注意:只能获得当前鼠标样式然后再重新设置鼠标样式
    tempCursor = cursor();                                 //因为获得的不是鼠标指针，所以不能这样用:cursor().setXXXXX

    if(diffTop < edgeMargin){                              //根据 边缘距离 分类改变尺寸的方向
        if(diffLeft < edgeMargin){
            resizeDir = topLeft;
            tempCursor.setShape(Qt::SizeFDiagCursor);
        }
        else if(diffRight < edgeMargin){
            resizeDir = topRight;
            tempCursor.setShape(Qt::SizeBDiagCursor);
        }
        else{
            resizeDir = top;
            tempCursor.setShape(Qt::SizeVerCursor);
        }
    }
    else if(diffBottom < edgeMargin){
        if(diffLeft < edgeMargin){
            resizeDir = bottomLeft;
            tempCursor.setShape(Qt::SizeBDiagCursor);
        }
        else if(diffRight < edgeMargin){
            resizeDir = bottomRight;
            tempCursor.setShape(Qt::SizeFDiagCursor);
        }
        else{
            resizeDir = bottom;
            tempCursor.setShape(Qt::SizeVerCursor);
        }
    }
    else if(diffLeft < edgeMargin){
        resizeDir = left;
        tempCursor.setShape(Qt::SizeHorCursor);
    }
    else if(diffRight < edgeMargin){
        resizeDir = right;
        tempCursor.setShape(Qt::SizeHorCursor);
    }
    else{
        resizeDir = nodir;
        tempCursor.setShape(Qt::ArrowCursor);
    }

    setCursor(tempCursor);                    //重新设置鼠标,主要是改样式
}

void DataEntryDialog::mouseMoveEvent(QMouseEvent * event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){                 //如果左键是按下的
      if(resizeDir == nodir){                             //如果鼠标不是放在边缘那么说明这是在拖动窗口
         move(event->globalPos() - dragPosition);
      }
      else{
          int ptop,pbottom,pleft,pright;                   //窗口上下左右的值
          ptop = frameGeometry().top();
          pbottom = frameGeometry().bottom();
          pleft = frameGeometry().left();
          pright = frameGeometry().right();
          if(resizeDir & top){                               //检测更改尺寸方向中包含的上下左右分量
              if(height() == minimumHeight()){
                  ptop = min(event->globalY(),ptop);
              }
              else if(height() == maximumHeight()){
                  ptop = max(event->globalY(),ptop);
              }
              else{
                  ptop = event->globalY();
              }
          }
          else if(resizeDir & bottom){
              if(height() == minimumHeight()){
                  pbottom = max(event->globalY(),ptop);
              }
              else if(height() == maximumHeight()){
                  pbottom = min(event->globalY(),ptop);
              }
              else{
                  pbottom = event->globalY();
              }
          }

          if(resizeDir & left){                        //检测左右分量
              if(width() == minimumWidth()){
                  pleft = min(event->globalX(),pleft);
              }
              else if(width() == maximumWidth()){
                  pleft = max(event->globalX(),pleft);
              }
              else{
                  pleft = event->globalX();
              }
          }
          else if(resizeDir & right){
              if(width() == minimumWidth()){
                  pright = max(event->globalX(),pright);
              }
              else if(width() == maximumWidth()){
                  pright = min(event->globalX(),pright);
              }
              else{
                  pright = event->globalX();
              }
          }
          setGeometry(QRect(QPoint(pleft,ptop),QPoint(pright,pbottom)));
      }
  }
  else testEdge();   //当不拖动窗口、不改变窗口大小尺寸的时候  检测鼠标边缘
}

void DataEntryDialog::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
    if(resizeDir != nodir){         //还原鼠标样式
        testEdge();
    }
    qDebug() << this->width() << this->height() << "长度，宽度"  << endl;
}


void DataEntryDialog::on_partDelePushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("确定删除分区信息吗");
   // msgBox.setInformativeText("将同时删除分区下的防区信息");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    QSqlQuery query;
    QModelIndexList indexList = ui->partTableView->selectionModel()->selectedRows(0);
    QModelIndex index;
    QString sql;
    QList<int> deleList;
    foreach (index, indexList) {
        //查询防区列表
        int partNum = index.data().toInt();
        sql = QString("SELECT * FROM alarmImfor WHERE PART_NUM = %1").arg(partNum);
        query.exec(sql);
        while(query.next())
        {
            int n = query.value(0).toInt();
            deleList.append(n);
        }
        query.exec(QString("DELETE FROM alarmImfor WHERE part_num = %1").arg(partNum));
        partModel.removeRow(index.row());

    }
    foreach(int num,deleList)
    {
        query.exec(QString("DELETE FROM pointOnMap WHERE NUM = %1").arg(num));
    }
    alarmModel.select();
    partModel.select();
    emit deleNumList(deleList);
}

void DataEntryDialog::on_partAddPushButton_clicked()
{
    EntryForm *form = new EntryForm(this,&partModel);
    form->exec();
    delete form;
}

void DataEntryDialog::closeEvent(QCloseEvent *)
{
   if(mType == MachineType::KS_500A)
   {
       emit dataChange();
   }

}

void DataEntryDialog::showEvent(QShowEvent *)
{

}

void DataEntryDialog::on_partSaveButton_clicked()
{
    int row = ui->partTableView->currentIndex().row();
    EntryForm *form = new EntryForm(this,&partModel,row);
    form->exec();
    delete form;
}

void DataEntryDialog::on_alarmAddpushButton_clicked()
{
    EntryForm *form = new EntryForm(this,&alarmModel,-1,EntryForm::ALARM_FORM);
    form->exec();
    delete form;
}



void DataEntryDialog::on_alarmEditPushButton_clicked()
{
    int row = ui->alarmTableView->currentIndex().row();
    if(row == -1)
    {
        return;
    }
    EntryForm *form = new EntryForm(this,&alarmModel,row,EntryForm::ALARM_FORM);
    form->exec();
    delete  form;
}

void DataEntryDialog::on_alarmDelPushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("确定删除防区信息");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
    {
        return;
    }
    QSqlQuery query;
    QModelIndexList indexList = ui->alarmTableView->selectionModel()->selectedRows();
    QModelIndex index;
    int n;
    QList<int> deleList;
    foreach (index, indexList) {
        n = alarmModel.index(index.row(),0).data().toInt();
        alarmModel.removeRow(index.row());
        query.exec(QString("DELETE FROM pointOnMap WHERE NUM = %1").arg(n));
        deleList << n;
    }
    emit deleNumList(deleList);
    alarmModel.select();
}

void DataEntryDialog::on_pushButton_clicked()
{
    mapper.submit();
}


bool DataEntryDialog::eventFilter(QObject *obj, QEvent *event)
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

void DataEntryDialog::on_closePushButton_clicked()
{
    this->close();
}
