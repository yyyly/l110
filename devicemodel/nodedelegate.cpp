#include "nodedelegate.h"
#include "node.h"
#include <QRect>
#include <QMargins>
#include <QFontMetrics>
#include <QBrush>
#include <QColor>
#include <QStyleOptionViewItem>

NodeDelegate::NodeDelegate()
{
    QPixmap *icon = nullptr;
    icon = new QPixmap(":/image/defence.png");
    iconVector.append(icon);
    icon = new QPixmap(":/image/outDefence.png");
    iconVector.append(icon);
    icon = new QPixmap(":/image/bypass.png");
    iconVector.append(icon);
    icon = new QPixmap(":/image/noStatu.png");
    iconVector.append(icon);
}

void NodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.model()->data(index,Qt::DisplayRole).toString();
    Node::NodeType type = (Node::NodeType)index.model()->data(index,Qt::UserRole + 2).toInt();
    switch (type) {
    case Node::Alarm:
    {
        QRect rect = option.rect;
        int h = rect.height();
        int x1 = rect.x();
        int y1 = rect.y();
        QRect iconRect = QRect(x1,y1,h,h);
        QRect displayRect = rect - QMargins(h,0,0,0);
        int s = (int)index.model()->data(index,Qt::UserRole + 1).toInt() - 4;
        drawDecoration(painter,option,iconRect,*iconVector.at(s));
        drawDisplay(painter,option,displayRect,text);
    }
        break;
    case Node::Partition:
    {
        QString str;
        QStyleOptionViewItem myOption = option;
        Node::Statu statu = (Node::Statu)index.model()->data(index,Qt::UserRole +1).toInt();
        int textSize = option.fontMetrics.width(text);
        QRect backgroundRect = option.rect - QMargins(textSize,0,0,0);
        myOption.rect = backgroundRect;
        if(statu == Node::Defence)
        {
            myOption.backgroundBrush = QBrush(Qt::red);
        }
        if(statu == Node::OutDefence)
        {
            myOption.backgroundBrush = QBrush(Qt::green);
        }
        drawBackground(painter,myOption,index);
        drawDisplay(painter,option,option.rect,text);
    }
        break;
    case Node::Device:
    {

    }
    default:
    {
        drawDisplay(painter,option,option.rect,text);
    }
        break;
    }


}

QSize NodeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant value = index.data(Qt::SizeHintRole);
    QSize size;
    if (value.isValid())
    {
        size = qvariant_cast<QSize>(value);
        return size;
    }
    QRect decorationRect = rect(option, index, Qt::DecorationRole);
    QRect displayRect = rect(option, index, Qt::DisplayRole);
    QRect checkRect = rect(option, index, Qt::CheckStateRole);

    doLayout(option, &checkRect, &decorationRect, &displayRect, true);
    size =  (decorationRect|displayRect|checkRect).size();
    return size;
}
