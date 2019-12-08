#ifndef DEVICETREEMODE_H
#define DEVICETREEMODE_H

#include <QObject>
#include <QStandardItemModel>
#include <QVariantMap>
#include <QString>
#include <QVariantList>
#include "sqldata.h"

class DeviceTreeMode : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DeviceTreeMode(QMap<int,CameraDeviceImf> &dataMap,QObject *parent = 0);
    void setModeData(QMap<int,CameraDeviceImf> &dataMap);

    QVariantList* getCameraList();
    QStandardItem *findItem(const QString text);
    QStandardItem *findItem(int serial,int channelNum = 0);
    QStandardItem *addItem(const CameraDeviceImf& imf);
    QStandardItem *removeItem(int serial);
public slots:
    void changeItemState(CameraDeviceImf *imf, Camero::LineState state = Camero::OffLine);
private:
    QVariantMap dataMap;
    QVariantList cameraList;
    QList<QStandardItem *> pDeviceItemList;
    int serchNum;
    QString currentSerText;
    QStandardItem *firstItem;
};

#endif // DEVICETREEMODE_H
