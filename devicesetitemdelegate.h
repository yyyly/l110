#ifndef DEVICESETITEMDELEGATE_H
#define DEVICESETITEMDELEGATE_H

#include<QItemDelegate>
#include<QStyleOptionViewItem>

class DeviceSetItemDelegate : public QItemDelegate
{
public:
    DeviceSetItemDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,const QModelIndex &index) const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;
private:
    static const QStringList alarmTypeStringList;
    static const QStringList partStringList;
public slots:
    void commitAndCloseEditor();
};

#endif // DEVICESETITEMDELEGATE_H
