#include "sqldata.h"
#include <QSqlQuery>
#include <QVariant>



SqlData::SqlData()
{
   int serial;
   CameraDeviceImf deviceImf;
   QSqlQuery query;
   query.exec("SELECT * FROM deviceImform");
   while(query.next())
   {
       serial = query.value(0).toInt();
       deviceImf.mold = (Camero::Mold)query.value(1).toInt();
       deviceImf.name = query.value(2).toString();
       deviceImf.ip = query.value(3).toString();
       deviceImf.port = query.value(4).toInt();
       deviceImf.accoumt = query.value(5).toString();
       deviceImf.passWord = query.value(6).toString();
       deviceImf.type = (Camero::TYPE)query.value(7).toInt();
       deviceImf.channalAmount = query.value(8).toInt();
       deviceImf.luserId = -1;
       deviceImf.playId = -1;
       cameraImfMap[serial] = deviceImf;
   }
   query.exec("SELECT * FROM channelName");
   int channelNum;
   QString channelName;
   while(query.next())
   {
       serial = query.value(0).toInt();
       channelNum = query.value(1).toInt();
       channelName = query.value(2).toString();
       cameraImfMap[serial].channelNameMap[channelNum] = channelName;
   }
}

QMap<int,CameraDeviceImf> &SqlData::getCameraDeviceImf()
{
    return cameraImfMap;
}
