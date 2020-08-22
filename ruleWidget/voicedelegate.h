#ifndef VOICEDELEGATE_H
#define VOICEDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QModelIndex>

class VoiceDelegate : public QItemDelegate
{
public:
    VoiceDelegate(int VoiceColumn,QObject *parent = 0);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    int VoiceColumn = 2;
};

#endif // VOICEDELEGATE_H
