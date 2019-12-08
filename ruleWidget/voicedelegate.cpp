#include "voicedelegate.h"
#include<QString>
#include<QStyleOptionButton>
#include<QApplication>
VoiceDelegate::VoiceDelegate(int VoiceColumn, QObject *parent)
    :QItemDelegate(parent)
{

}

void VoiceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*if(index.column() == VoiceColumn)
       {
           QString text = index.model()->data(index).toString();
           QStyleOptionViewItem myOption(option);
           myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
           drawDisplay(painter,myOption,myOption.rect,text);
           //drawFocus(painter,myOption,myOption.rect);
           if(option.state & QStyle::QStyle::State_MouseOver)
           {
               QStyleOptionButton *button = new QStyleOptionButton();
               button->rect = option.rect.adjusted(145, 2, -5, -2);
               button->state |= QStyle::State_Enabled;
               button->SO_ToolBar;
               painter->save();
               painter->fillRect(button->rect, option.palette.highlight());
               QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);
               painter->restore();

           }
        }
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
     {
            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
         }*/
         QItemDelegate::paint(painter, option, index);
}
