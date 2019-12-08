#include "viewitemdelege.h"
#include <QApplication>
#include <QStyleOptionButton>
#include <QMouseEvent>
#include <QToolTip>

enum {
    VEDIO_PATH = 17
};

ViewItemDelege::ViewItemDelege(QObject *parent)
    :QItemDelegate (parent),
      buttonWidth(25),
      buttonHeight(20),
      playButton(new QPushButton())
{
    playButton->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/image/viewPlay.png);} \
                              QPushButton:hover {image:url(:/image/play_haver.png);} \
                              QPushButton:pressed {image:url(:/image/play_haver.png);}");
}

void ViewItemDelege::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == VEDIO_PATH)
    {
        QString str = index.data(Qt::DisplayRole).toString();
        if(!str.isEmpty())
        {
            drawBackground(painter,option,index);
            //画按键
            int half = (option.rect.width() - buttonWidth) / 2;
            int top = (option.rect.height() - buttonHeight) / 2;
            QStyleOptionButton buttonOption;
            buttonOption.rect  = QRect(option.rect.left() + half,option.rect.top() + top,buttonWidth,buttonHeight);
            buttonOption.state |= QStyle::State_Enabled;
            if(buttonOption.rect.contains(mouse_point))
            {
                if(state == 0)
                {
                    buttonOption.state |= QStyle::State_MouseOver;
                }
                else if(state == 1){

                    buttonOption.state |= QStyle::State_Sunken;
                }
            }
            QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter,playButton.data());
        }
        else {
            QItemDelegate::paint(painter, option, index);
            QApplication::style()->drawItemText(painter,option.rect,Qt::AlignCenter,option.palette,true,"无录像文件");
        }
    }
    else {
        QItemDelegate::paint(painter, option, index);
    }
}

bool ViewItemDelege::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(!(index.column() == VEDIO_PATH))
        return  false;
    state = -1;
    bool bRepaint = false;
    QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
    mouse_point = pEvent->pos();
    QApplication::restoreOverrideCursor();
    QStyleOptionButton buttonOption;
    int half = (option.rect.width() - buttonWidth) / 2;
    int top = (option.rect.height() - buttonHeight) / 2;
    buttonOption.rect  = QRect(option.rect.left() + half,option.rect.top() + top,buttonWidth,buttonHeight);
    if (!buttonOption.rect.contains(mouse_point) || index.data().toString().isEmpty())
        return bRepaint;
    bRepaint = true;
            switch (event->type())
            {
                // 鼠标滑过
                case QEvent::MouseMove:
                {
                    // 设置鼠标样式为手型
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);

                    state = 0;
                    QToolTip::showText(pEvent->globalPos(), "播放");
                    break;
                }
                // 鼠标按下
                case QEvent::MouseButtonPress:
                {
                    state = 1;
                    break;
                }
                // 鼠标释放
                case QEvent::MouseButtonRelease:
                {
                    emit play(index);
                    break;
                }
                default:
                    break;
            }

        return bRepaint;

}

