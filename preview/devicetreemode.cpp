# include "devicetreemode.h"
#include<QJsonParseError>
#include<QJsonDocument>
#include<QVariantList>
#include<QDebug>
#include<QStandardItem>
#include<QIcon>
#include "globdata.h"
DeviceTreeMode::DeviceTreeMode(QMap<int, CameraDeviceImf> &dataMap, QObject *parent)
{
    setModeData(dataMap);
    serchNum = 0;
    firstItem = nullptr;
}

void DeviceTreeMode::setModeData(QMap<int, CameraDeviceImf> &dataMap)
{
    QStandardItem *rootItem = this->invisibleRootItem();
    QStandardItem *Item = new QStandardItem("视频设备");
    Item->setIcon(QIcon(":/image/Group.png"));
    Item->setEditable(false);
    rootItem->appendRow(Item);
    QStandardItem *pDeviceItem = nullptr;
    QStandardItem *pCameraItem = nullptr;
    CameraDeviceImf deviceImf;
    foreach (deviceImf, dataMap)
    {
            pDeviceItem = new QStandardItem(deviceImf.name);
            int key = dataMap.key(deviceImf);
            pDeviceItem->setData(key,Qt::UserRole + 5);
            pDeviceItem->setData(1,Qt::UserRole + 6);
            pDeviceItem->setEditable(false);
            Item->appendRow(pDeviceItem);
            if(deviceImf.channalAmount > 1)
            {
                QString channelName;
                for(int i =1;i <= deviceImf.channalAmount;i++)
                {
                    if(!deviceImf.channelNameMap.value(i).isEmpty())
                    {
                        channelName = deviceImf.channelNameMap.value(i);
                    }
                    else
                    {
                        channelName = QString("第%1通道").arg(i);
                    }
                    pCameraItem = new QStandardItem(channelName);
                    pCameraItem->setData(key,Qt::UserRole + 5);
                    pCameraItem->setData(i,Qt::UserRole + 6);
                    pCameraItem->setEditable(false);
                    pCameraItem->setIcon(QIcon(":/image/TreeIcon/channelOff.png"));
                    pDeviceItem->appendRow(pCameraItem);

                }
                pDeviceItem->setIcon(QIcon(":/image/TreeIcon/nvrOffline.png"));
            }
            else
                pDeviceItem->setIcon(QIcon(":/image/TreeIcon/cameralOffline.png"));
            pDeviceItemList << pDeviceItem;
    }
}

QVariantList* DeviceTreeMode::getCameraList()
{
    QVariantList *list = &cameraList;
    return list;
}

void DeviceTreeMode::changeItemState(CameraDeviceImf *imf, Camero::LineState state)
{
    imf->lineState = state;
    int serial = deviceImf.key(*imf);
    QStandardItem *pItem = nullptr;
    foreach(pItem,pDeviceItemList)
    {
        if(pItem->data(Qt::UserRole + 5) == serial)
            break;
    }
    if(pItem == nullptr)
        return;
    pItem->setData(imf->name,Qt::EditRole);
    if(state == Camero::OnLine)//在线
    {
        if(imf->channalAmount > 1 && imf->channalAmount == pItem->rowCount())
        {
            pItem->setIcon(QIcon(":/image/TreeIcon/NVROnline.png"));
            int i = 0;
            while (pItem->child(i,0)) {
              QStandardItem  *pCItem = pItem->child(i,0);
              pCItem->setIcon(QIcon(":/image/TreeIcon/channel.png"));
              i++;
            }
        }
        else if(imf->channalAmount > 1 && imf->accoumt != pItem->rowCount())
        {
            //先删除节点的所有子节点
            int j = 0;
            while (pItem->child(j,0)) {
                pItem->removeRow(j);
            }
            //再重新添加
            QString channelName;
            QStandardItem *pCameraItem = nullptr;
            int key = pItem->data(Qt::UserRole + 5).toInt();
            for(int i =1;i <= imf->channalAmount;i++)
            {
                if(!imf->channelNameMap.value(i).isEmpty())
                {
                    channelName = imf->channelNameMap.value(i);
                }
                else
                {
                    channelName = QString("第%1通道").arg(i);
                }
                pCameraItem = new QStandardItem(channelName);
                pCameraItem->setData(key,Qt::UserRole + 5);
                pCameraItem->setData(i,Qt::UserRole + 6);
                pCameraItem->setEditable(false);
                pCameraItem->setIcon(QIcon(":/image/TreeIcon/channel.png"));
                pItem->appendRow(pCameraItem);

            }
            pItem->setIcon(QIcon(":/image/TreeIcon/NVROnline.png"));

        }
        else {
            pItem->setIcon(QIcon(":/image/TreeIcon/cameralOnline.png"));
            int i = 0;
            while (pItem->child(i,0)) {
                pItem->removeRow(i);
            }
        }
    }
    else//离线
    {
        if(imf->channalAmount > 1 && imf->channalAmount == pItem->rowCount())//ipc变NVR或者NVR路数有变化
        {
            pItem->setIcon(QIcon(":/image/TreeIcon/nvrOffline.png"));
            int i = 0;
            while (pItem->child(i,0)) {
              QStandardItem  *pCItem = pItem->child(i,0);
              pCItem->setIcon(QIcon(":/image/TreeIcon/channelOff.png"));
              i++;
            }
        }
        else if(imf->channalAmount > 1 && imf->accoumt != pItem->rowCount())
        {
            //先删除节点的所有子节点
            int j = 0;
            while (pItem->child(j,0)) {
                pItem->removeRow(j);
            }
            //再重新添加
            QString channelName;
            QStandardItem *pCameraItem = nullptr;
            int key = pItem->data(Qt::UserRole + 5).toInt();
            for(int i =1;i <= imf->channalAmount;i++)
            {
                if(!imf->channelNameMap.value(i).isEmpty())
                {
                    channelName = imf->channelNameMap.value(i);
                }
                else
                {
                    channelName = QString("第%1通道").arg(i);
                }
                pCameraItem = new QStandardItem(channelName);
                pCameraItem->setData(key,Qt::UserRole + 5);
                pCameraItem->setData(i,Qt::UserRole + 6);
                pCameraItem->setEditable(false);
                pCameraItem->setIcon(QIcon(":/image/TreeIcon/channelOff.png"));
                pItem->appendRow(pCameraItem);

            }
            pItem->setIcon(QIcon(":/image/TreeIcon/nvrOffline.png"));

        }
        else {//NVR变IPC
            pItem->setIcon(QIcon(":/image/TreeIcon/cameralOffline.png"));
            int i = 0;
            while (pItem->child(i,0)) {
                pItem->removeRow(i);
            }
        }
    }

}

QStandardItem * DeviceTreeMode::findItem(const QString text)
{
    if(currentSerText == text)//说明是第二次或更多次搜索
    {
         serchNum++;
    }
    else {
        currentSerText = text;
        serchNum = 0;
    }
    QStandardItem *item = nullptr;
    int j = 0;
    foreach (item, pDeviceItemList) {

        if(item->text().contains(text))
        {
            j++;
            if(j > serchNum)
            {
                if(serchNum == 0)
                {
                    firstItem = item;
                }
               return  item;
            }


        }
        int i = 0;
        while (item->child(i,0)) {
          QStandardItem *citem = item->child(i,0);
          if(citem->text().contains(text))
          {
              j++;
              if(j > serchNum)
              {
                  if(serchNum == 0)
                  {
                      firstItem = citem;
                  }
                  return  citem;
              }
          }
          i++;
        }
    }
    serchNum = 0;
    return  firstItem;
}

QStandardItem * DeviceTreeMode::findItem(int serial, int channelNum)
{
    QStandardItem *invisiblerootItem = this->invisibleRootItem();
    QStandardItem *rootItem = invisiblerootItem->child(0);
    QStandardItem *dItem;
    int i = 0;
    while (rootItem->child(i)) {
        dItem = rootItem->child(i);
        if(dItem->data(Qt::UserRole + 5).toInt() == serial)
        {
            break;
        }
        i++;
    }
    if(i == rootItem->rowCount())
    {
        if(dItem->data(Qt::UserRole + 5).toInt() != serial);//说明没有找到想
        return  nullptr;
    }
    if(channelNum == 0)
        return dItem;
    QStandardItem *cItem = dItem->child(channelNum - 1);
    return cItem;
}

QStandardItem * DeviceTreeMode::addItem(const CameraDeviceImf &imf)
{
    QStandardItem *invisiblerootItem = this->invisibleRootItem();
    QStandardItem *rootItem = invisiblerootItem->child(0);
    QStandardItem * pDeviceItem = new QStandardItem(imf.name);
    int key = deviceImf.key(imf);
    pDeviceItem->setData(key,Qt::UserRole + 5);
    pDeviceItem->setData(1,Qt::UserRole + 6);
    pDeviceItem->setEditable(false);
    rootItem->appendRow(pDeviceItem);
    if(imf.channalAmount > 1)
    {
        QString channelName;
        for(int i =1;i <= imf.channalAmount;i++)
        {
            if(!imf.channelNameMap.value(i).isEmpty())
            {
                channelName = imf.channelNameMap.value(i);
            }
            else
            {
                channelName = QString("第%1通道").arg(i);
            }
            QStandardItem * pCameraItem = new QStandardItem(channelName);
            pCameraItem->setData(key,Qt::UserRole + 5);
            pCameraItem->setData(i,Qt::UserRole + 6);
            pCameraItem->setEditable(false);
            pCameraItem->setIcon(QIcon(":/image/TreeIcon/channelOff.png"));
            pDeviceItem->appendRow(pCameraItem);

        }
        pDeviceItem->setIcon(QIcon(":/image/TreeIcon/nvrOffline.png"));
    }
    else
        pDeviceItem->setIcon(QIcon(":/image/TreeIcon/cameralOffline.png"));
    pDeviceItemList << pDeviceItem;
}

QStandardItem *DeviceTreeMode::removeItem(int serial)
{
    QStandardItem *item = findItem(serial);
    if(item == nullptr)
        return nullptr;
    QStandardItem *invisiblerootItem = this->invisibleRootItem();
    QStandardItem *rootItem = invisiblerootItem->child(0);
    int row = 0;
    while (rootItem->child(row)) {
        if(rootItem->child(row) == item)
        {
            break;
        }
        row++;
    }
    rootItem->removeRow(row);
    pDeviceItemList.removeAt(row);
    return rootItem->child(row);
}
