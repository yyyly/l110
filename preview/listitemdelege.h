#ifndef LISTITEMDELEGE_H
#define LISTITEMDELEGE_H

#include <QObject>
#include <QItemDelegate>
#include <QStringList>

class ListItemDelege : public QItemDelegate
{
public:
    ListItemDelege(QObject *parent,const QStringList &stringList);
protected:

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    const QStringList *pStringList;
};

#endif // LISTITEMDELEGE_H
