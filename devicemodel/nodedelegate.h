#ifndef NODEDELEGATE_H
#define NODEDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QVector>
#include <QVector>

class NodeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    NodeDelegate();
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QVector<QPixmap *> iconVector;
};

#endif // NODEDELEGATE_H
