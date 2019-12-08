#ifndef SQLDATA_H
#define SQLDATA_H
#include <QMap>

#include"cameronet.h"

class SqlData
{
public:
    SqlData();
    QMap<int, CameraDeviceImf> &getCameraDeviceImf();
private:
    QMap<int,CameraDeviceImf> cameraImfMap;
};

#endif // SQLDATA_H
