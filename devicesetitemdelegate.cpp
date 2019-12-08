#include "devicesetitemdelegate.h"
#include "z1801.h"
#include<QComboBox>

const QStringList DeviceSetItemDelegate::alarmTypeStringList = {"防区无用","出/入口延时防区","周边防区","内部防区",
                                                      "日/夜防区","24小时无声防区","24小时有声防区","内部防区(延时)",
                                                      "火警防区","气感防区","医疗防区","门铃防区","防盗防区"};
const QStringList DeviceSetItemDelegate::partStringList = {"分区1","分区2","分区3","分区4","分区5","分区6",
                                                          "分区7","分区8"};

DeviceSetItemDelegate::DeviceSetItemDelegate(QObject *parent)
    :QItemDelegate(parent)
{

}

QWidget* DeviceSetItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *box = new QComboBox(parent);
    return box;
}

void DeviceSetItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 0)
        return;
    QString currentStr = index.model()->data(index,Qt::DisplayRole).toString();
    QComboBox *box = qobject_cast<QComboBox *>(editor);
    if(index.column() ==1)
    {
        box->addItems(alarmTypeStringList);
        box->setCurrentText(currentStr);
    }
    else if(index.column() ==2)
    {
        box->addItems(partStringList);
        box->setCurrentText(currentStr);
    }
    else if(index.column() == 3 || index.column() == 4)
    {
        box->addItem("是");
        box->addItem("否");
        box->setCurrentText(currentStr);
    }
    else
    {
        box->addItem("Z851A");
        box->addItem("Z861A");
        box->setCurrentText(currentStr);
    }


}

void DeviceSetItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = qobject_cast<QComboBox *>(editor);
    if(index.column() == 1)
    {

        QString str = box->currentText();
        int i = 0;
        QString s;
        foreach (s, alarmTypeStringList) {
            if(str == s)
            {
                break;
            }
            i++;
        }
        model->setData(index,i);
    }
    else if(index.column() == 2)
    {
        QString str = box->currentText();
        int i = 1;
        QString s;
        foreach (s, partStringList) {
            if(str == s)
            {
                break;
            }
            i++;
        }
        model->setData(index,i);

    }
    else if(index.column() == 3 || index.column() == 4)
    {
        QString str = box->currentText();
        if(str == "是")
        {
            model->setData(index,true);
        }
        else
        {
            model->setData(index,false);
        }
    }
    else
    {
        QString str = box->currentText();
        if(str == "Z851A")
            model->setData(index,(int)ModuleType::Z851A);
        else {
            model->setData(index,(int)ModuleType::Z861A);
        }
    }

}

void DeviceSetItemDelegate::commitAndCloseEditor()
{

}
