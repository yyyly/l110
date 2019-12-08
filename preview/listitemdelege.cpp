#include "listitemdelege.h"

ListItemDelege::ListItemDelege(QObject *parent, const QStringList &stringList)
    :QItemDelegate(parent),
      pStringList(&stringList)
{

}




void ListItemDelege::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int n = index.data().toInt();
    QString text = pStringList->at(n);
    drawDisplay(painter,option,option.rect,text);
}
