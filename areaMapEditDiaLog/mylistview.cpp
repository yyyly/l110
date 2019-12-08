#include "mylistview.h"
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlQuery>
#include "globdata.h"



MyListView::MyListView(QWidget *parent)
    :QListView(parent)
{
    setAcceptDrops(true);

}

void MyListView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //setCursor(Qt::ClosedHandCursor);
        startPos = event->pos();
    }
    QListView::mousePressEvent(event);
}

void MyListView::mouseReleaseEvent(QMouseEvent *e)
{
    //setCursor(Qt::OpenHandCursor);
}

void MyListView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {

        int distance = (event->pos() - startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())
        {
            perforDrag();
        }
    }
    QListView::mouseMoveEvent(event);
}

void MyListView::dragEnterEvent(QDragEnterEvent *event)
{
    event->setDropAction(Qt::MoveAction);
    event->acceptProposedAction();
}

void MyListView::dragMoveEvent(QDragMoveEvent *e)
{
    e->setDropAction(Qt::MoveAction);
    e->accept();
}

void MyListView::dropEvent(QDropEvent *e)
{
    QWidget *source = qobject_cast<QWidget *>(e->source());
    if(source && source != this)
    {
        int i = e->mimeData()->text().toInt();
        emit addItem(i);
        emit dropFinished();
    }
}

void MyListView::perforDrag()
{
    QModelIndex index = currentIndex();
    if(index.isValid())
    {
        QString str = index.data(Qt::EditRole).toString();
        int i = index.data(Qt::EditRole).toInt();
        //AlarmType type = typeMap.value(i);
        AlarmType type;
        QSqlQuery query;
        query.exec(QString("SELECT ALARM_TYPE FROM alarmImfor WHERE ALARM_NUM = %1").arg(i));
        if(query.next())
        {
            type = (AlarmType)query.value(0).toInt();
        }
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(str);
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        switch (type) {
        case AlarmType::FIRE:
        {
            drag->setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
        }
            break;
        case AlarmType::STONE_ALARM:
        {
            drag->setPixmap(QPixmap(":/areaMapEditDiaLog/images/alarm_normal.png"));
        }
            break;
        default:
            break;
        }

        //drag->setDragCursor(QPixmap(":/images/dingwei.png"),Qt::MoveAction);
        if(drag->exec(Qt::MoveAction) == Qt::MoveAction)
        {
            qDebug() << "drag finished" << endl;
        }
    }
}
