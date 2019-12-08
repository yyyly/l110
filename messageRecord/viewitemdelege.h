#ifndef VIEWITEMDELEGE_H
#define VIEWITEMDELEGE_H

#include<QItemDelegate>
#include<QPoint>
#include<QScopedPointer>
#include<QPushButton>

class ViewItemDelege : public QItemDelegate
{
    Q_OBJECT
public:
    ViewItemDelege(QObject *parent);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
private:
    QPoint mouse_point;
    QScopedPointer<QPushButton> playButton;
    int buttonWidth;
    int buttonHeight;
    int state;//鼠标状态
signals:
    void play(const QModelIndex &index);
};

#endif // VIEWITEMDELEGE_H
