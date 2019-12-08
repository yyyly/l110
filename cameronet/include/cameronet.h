#ifndef CAMERONET_H
#define CAMERONET_H
#include<QString>
#include<QMap>
#include<QList>
#include<QObject>

#include "HCNetSDK.h"
#include "cameronet_global.h"
#include "enums.h"

struct CameraDeviceImf
{
    Camero::Mold mold;  //设备厂家
    QString name;  //设备名称
    QString ip;  //设备的IP地址
    unsigned short port;  //设备端口号
    QString accoumt;  //设备账户
    QString passWord;  //访问密码
    Camero::TYPE type;  //设备种类（路线机，摄像头）
    int channalAmount;  //（通道号）
    QMap<int,QString> channelNameMap;  //设备的通道名称
    LONG luserId;  //设备登陆后的句柄
    LONG playId;  //播放句柄
    DWORD LoginWarming;  //登陆时错误码
    DWORD playWarming;  //播放时错误码
    friend bool operator ==(const CameraDeviceImf &,const CameraDeviceImf &);
};
inline bool operator == (const CameraDeviceImf &imf1,const CameraDeviceImf &imf2)
{

 return imf1.name == imf2.name &&
        imf1.mold == imf2.mold &&
        imf1.ip == imf2.ip &&
        imf1.port == imf2.port&&
        imf1.type == imf2.type;
}

class CAMERONETSHARED_EXPORT CameroNet : public QObject
{

    Q_OBJECT
public:
    /*构造函数需实现单列模式*/
    explicit CameroNet(QObject *p =nullptr);

    ~CameroNet();

    /*登陆函数*/
    void login(CameraDeviceImf &i);

    /*播放函数，默认采用主码流，TCP连接方式*/
    LONG realPlay(CameraDeviceImf *info, LONG channel,HWND playWnd);

    /*停止播放，函数的错误信息留给调用模块处理*/
    DWORD stopPlay(CameraDeviceImf *info);

    /*登出，函数的错误信息留给调用模块处理*/
    DWORD loginOut(CameraDeviceImf *info);

    /*抓图函数*/
    DWORD capPic(CameraDeviceImf *info,const QString ficAdd);

    /*声音控制函数*/

    DWORD opendSound(CameraDeviceImf *info);
    DWORD closeSound(CameraDeviceImf *info);

signals:
    void signalDeviceStatusChanged();
protected:
    bool event(QEvent *event);
private:
    static void  CALLBACK hkLoginResutCallback(LONG lUserID,DWORD dwResult,
                                               LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo,void *pUser);
    static QList<CameraDeviceImf *> HKPinfoList;//存储登陆信息，析构函数使用
};

#endif // CAMERONET_H
