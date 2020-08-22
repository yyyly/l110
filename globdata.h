#ifndef GLOBDATA_H
#define GLOBDATA_H
#include <QMap>
#include <QString>
#include "preview/devicetreemode.h"
#include "z1801p.h"
#include "cameronet.h"

extern QMap<int,AlarmType> typeMap;
extern QMap<int,QString> nameMap;
extern QMap<int,CameraDeviceImf> deviceImf;
extern int page_max_count_record;
extern DeviceTreeMode *treeModle;
extern QString glob_user;
extern QString glob_nickName;
extern MachineType mType;
#endif // GLOBDATA_H
