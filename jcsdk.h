//--------------------------------------------------------------------------------------
// jcsdk.h
// SDK接口定义
// SDK interface definition
//--------------------------------------------------------------------------------------

#pragma once

#ifdef __cplusplus
#  define JCSDK_EXTERN extern "C"
#else
#  define JCSDK_EXTERN
#endif

#ifdef LINUX
#  define JCSDK_API JCSDK_EXTERN
#else
#  define JCSDK_API JCSDK_EXTERN __declspec(dllexport)
#endif

#ifdef LINUX
#  define JCSDK_CALLTYPE
#  define JVSDK_CALLBACKTYPE
#else
#  define JCSDK_CALLTYPE __stdcall
#  define JVSDK_CALLBACKTYPE __cdecl
#endif

#define JCSDK_INVALID_LINKVALUE -1
typedef int JCLink_t;

#define JCMAX_LINKNUM 512

#define JTRUE 1
#define JFALSE 0
typedef int JBOOL;

#ifdef LINUX

#  include <stdint.h>
	typedef uint8_t BYTE;
	typedef uint8_t UCHAR;
	typedef UCHAR *PUCHAR;
	typedef uint32_t DWORD;
	typedef void *DWORD_PTR;
	typedef void *LPVOID;
	typedef unsigned int UINT;
#else
#  include <WinSock2.h>
#  include <Windows.h>
#endif

//事件类型  Event Type
typedef enum _JCEventType
{
	//连接部分 Connect part
	JCET_Connected, //未使用 unused
	JCET_ConnectOK,	//连接成功  Connect successfully
	JCET_UserAccessError, //用户验证失败 Fail to user validation
	JCET_NoChannel,		//主控通道未开启  Master channel closed
	JCET_ConTypeError,	//连接类型错误 Connection type error
	JCET_ConCountLimit,	//超过主控连接最大数 excess Max. connection quantity of master
	JCET_ConTimeout,	//连接超时 Connection timeout
	JCET_DisconOK, //断开连接成功 Disconnected successfully
	JCET_ConAbout, //连接异常断开 Connection abnormally disconnected
	JCET_ServiceStop,	//主控断开连接 Master disconnected

	//远程录像相关 about remote record 
	JCET_GetFileListOK,		//获取远程录像成功 Get remote record successfully
	JCET_GetFileListError,	//获取远程录像失败 Fail to get remote record

	JCET_RemotePlayBegin,	//远程回放开始 Start remote playback
	JCET_RemotePlayEnd,		//远程回放结束 Remote playback end
	JCET_RemoteError,		//远程回放失败 Fail to remote playback
	JCET_RemotePlayTimeout,	//远程回访超时 Remote playback timeout

	//其他 Others
	JCET_StreamReset,	//码流重置信号(码流配置改变,须关闭并重新开启解码、录像等) 
						//Stream reset singals(stream setting was changed,it should close and restart decode,record etc._

	JCET_Alarm,	//报警信号 alarm singals
} JCEventType;

//下载事件类型  Download Event Type
typedef enum _JCDownloadEventType
{
	JCDET_DownloadData,		//录像下载数据 download record data
	JCDET_DownloadEnd,		//录像下载完成 finish downloading record
	JCDET_DownloadStop,		//录像下载停止 download stop
	JCDET_DownloadError,	//远程下载失败 Fail to download
	JCDET_DownloadTimeout,	//远程下载超时 Remote download timeout

} JCDownloadEventType;

//连接状态 Connection status
typedef enum _JCLinkStatus
{
	JCLS_UnConnect,	//未连接 unconnect
	JCLS_Connecting,//连接中 connecting
	JCLS_WaitData,	//连接成功等待数据 connect successfully,and wait for datas
	JCLS_Connected,	//已连接 connected
} JCLinkStatus;

//数据帧类型 Frame type
typedef enum _JCFrameType
{
	JCFT_I,			//视频I帧 Video frame rate I
	JCFT_P,			//视频P帧 Video frame rate P
	JCFT_B,			//视频B帧 Video frame rate B
	JCFT_Audio,		//音频帧  Audio frame rate
} JCFrameType;

//录像类型 Record type
typedef enum _JCRecFileType
{
	JCRT_Unknown = 0,//未知 unknown
	JCRT_SV4,		//sv4文件  sv4 file
	JCRT_SV5,		//sv5文件   sv5 file
	JCRT_SV6,		//sv6文件   sv6 file
	JCRT_MP4,		//MP4文件  MP4 file
    JCRT_JVFS,		//中维文件系统   jovision filesystem
} JCRecFileType;

//设备类型 Device type
typedef enum _JCDeviceType
{
	JCDT_All = 0,		//未知设备 unknow device
	JCDT_Card,			//视频采集卡 DVR card
	JCDT_DVR,			//DVR
	JCDT_IPC,			//IPC
	JCDT_NVR,			//NVR
} JCDeviceType;

//编码类型 Codec type
typedef enum _JCCodecID
{
	JCCID_Unknown,		//未知编码 unknow code

	//视频编码 video decode
	JCCID_JENC04,		//中维04版编码 JOVISION 04 version code
	JCCID_H264,			//H264编码  H.264 code

	//音频编码 audio decode
	JCCID_PCM,			//PCM数据 PCM data
	JCCID_AMR,			//AMR编码  AMR data
	JCCID_G711A,		//G711编码A decode A
	JCCID_G711U,		//G711编码U  decode U

	//语音对讲用   Two way audio
	JCCID_G729A_8k16b,	//PC板卡类产品用 used by DVR card based on PC 
	JCCID_PCM_8k8b,		//旧设备使用，新设备已弃用 Old device use,new device give up
	JCCID_G711A_8k16b,	//嵌入式设备(IPC/DVR/NVR)使用  Embedded device(IPC/DVR/NVR)use
	JCCID_G711U_8k16b,	//嵌入式设备(IPC/DVR/NVR)使用  Embedded device(IPC/DVR/NVR)use  



	JCCID_Undefined = 0xFF

} JCCodecID;

//远程回访控制命令 Remote palyback command
typedef enum _JCRemotePlayCommand
{
	JCRPC_Play,			//播放 Play
	JCRPC_Pause,		//暂停 Pause
	JCRPC_SPEEDUP,		//加速 Speedup
	JCRPC_SPEEDDOWN,	//减速  Speeddown
	JCRPC_SPEEDNORMAL,	//正常速度 Speed normal
	JCRPC_SeekPos,		//跳转到，参数部分传入要跳转到的帧数，有效取值1到录像文件最大帧数(可通过JCSDK_GetStreamInfo()获取)
} JCRemotePlayCommand;

//语音对讲事件类型  Talk event type
typedef enum _JCTalkEventType
{
	JCTET_Accepted,		//对方接受语音对讲  Accepted audio talk
	JCTET_Data,			//语音对讲数据 Audio talk datas
	JCTET_Stoped,		//语音对讲结束  Audio talk stopped

} JCTalkEventType;

//云台控制命令类型
typedef enum _JCPTZCmdType
{
	JCPCT_Up,			//上 Up
	JCPCT_Down,			//下 Down
	JCPCT_Left,			//左 Left
	JCPCT_Right,		//右 Right
	JCPCT_Auto,			//自动巡航 Auto
	JCPCT_IrisUp,		//光圈大 Iris Up
	JCPCT_IrisDown,		//光圈小 Iris Down
	JCPCT_FacolUp,		//变焦大 Facol Up
	JCPCT_FacolDown,	//变焦小 Facol Down
	JCPCT_ZoomUp,		//变倍大 Zoom Up
	JCPCT_ZoomDown,		//变倍小 Zoom Down
	JCPCT_Custom1,		//自定义功能1(具体功能由设备决定)
	JCPCT_Custom2,		//自定义功能2(具体功能由设备决定)
	JCPCT_Custom3,		//自定义功能3(具体功能由设备决定)
	JCPCT_Custom4,		//自定义功能4(具体功能由设备决定)

} JCPTZCmdType;

//码流帧 Stream Frame
typedef struct _JCStreamFrame
{
	JCFrameType sType;
	PUCHAR pFrameData;
	int nFrameSize;
}
JCStreamFrame, *PJCStreamFrame;

//检索设备信息 Search device information
typedef struct _JCLanDeviceInfo
{
	char szCloudSEE[16];
	char szIP[16];
	int nPort;
	char szDeviceName[128];
	int nDeviceType;
	int nChannelNum;
}
JCLanDeviceInfo, *PJCLanDeviceInfo;

//设备信息 device information
typedef struct _JCDeviceInfo
{
	JCDeviceType eDevType;
}
JCDeviceInfo, *PJCDeviceInfo;

//码流信息 stream information
typedef struct _JCStreamInfo
{
	JCCodecID eVideoCodec;		//视频编码  Video code
	int nFrameWidth;			//视频帧宽度 Video Frame width
	int nFrameHeight;			//视频帧高度 Video Frame Height
	int nVideoFrameRateNum;		//视频帧率分子 Video Frame rate numerator
	int nVideoFrameRateDen;		//视频帧率分母  Video Frame rate denominator

	JCCodecID eAudioCodec;		//音频编码 Audio code
	int nAudioSamples;			//音频采样率 Audio sample ratio
	int nAudioSampleBits;		//音频采样位数  Audio sample bits
	int nAudioChannels;			//音频通道数 Audio channels

	JCRecFileType eRecFileType;	//录像文件类型  Record file type
	int nRecFileTotalFrames;	//录像文件总帧数 Record file total frames

	int nRemotePlayCurPos;		//远程回放当前位置
}
JCStreamInfo, *PJCStreamInfo;

//日期段  Date block
typedef struct _JCDateBlock
{
	int nBeginYear;		//开始年份 Begin year
	int nBeginMonth;	//开始月份  Begin month
	int nBeginDay;		//开始日期  Begin day
	int nEndYear;		//结束年份  End year
	int nEndMonth;		//结束月份  End month
	int nEndDay;		//结束日期  End day
}
JCDateBlock, *PJCDateBlock;

//录像信息 Record file information
typedef struct _JCRecFileInfo
{
	int nRecFileID;
	JCRecFileType eType;
	char szBeginTime[8];
	char szFilename[16];
	char szPathName[64];
}
JCRecFileInfo, *PJCRecFileInfo;

//yv12数据帧 DWORD Frame
typedef struct _JCRawFrame
{
	DWORD dwWidth;
	DWORD dwHeight;
	PUCHAR pY;
	PUCHAR pU;
	PUCHAR pV;
	DWORD dwYPitch;
	DWORD dwUPitch;
	DWORD dwVPitch;
}
JCRawFrame, *PJCRawFrame;

//回调函数 Callback function
typedef void (JVSDK_CALLBACKTYPE *JCEventCallback_t)(JCLink_t nLinkID, JCEventType etType, DWORD_PTR pData1, DWORD_PTR pData2, LPVOID pUserData);
typedef void (JVSDK_CALLBACKTYPE *JCDataCallback_t)(JCLink_t nLinkID, PJCStreamFrame pFrame, LPVOID pUserData);
typedef void (JVSDK_CALLBACKTYPE *JCDownloadDataCallback_t)(JCLink_t nLinkID, JCDownloadEventType etType, DWORD dwProgress/* 0~10000, progress * 10000 */, DWORD dwFileSize, LPVOID pUserData);
typedef void (JVSDK_CALLBACKTYPE *JCRawDataCallback_t)(JCLink_t nLinkID, PJCRawFrame pFrame, LPVOID pUserData);
typedef void (JVSDK_CALLBACKTYPE *JCLanSearchDeviceCallback_t)(PJCLanDeviceInfo pDevice);
typedef void (JVSDK_CALLBACKTYPE *JCUserDataCallback_t)(JCLink_t nLinkID, PUCHAR pData, int nSize);
typedef void (JVSDK_CALLBACKTYPE *JCTalkCallback_t)(JCLink_t nLinkID, JCTalkEventType etType, JCCodecID cdCodec, PUCHAR pData, int nSize, LPVOID pUserData);

//全局函数  global function------------------------------------------------------------------------------
/**
* @brief 初始化SDK   Initialize SDK
* @param nLocStartPort 起始端口(默认使用-1即可) Start port (Default -1)
* @param szTempPath 临时文件目录(外部应保证目录已存在，传NULL使用默认目录) Temp files path(Default NULL)
* @return 返回操作结果 return to operation result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_InitSDK(int nLocStartPort, const char* szTempPath);

/**
* @brief 释放SDK release SDK
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_ReleaseSDK();

/**
* @brief 设置回调函数  Set callback function
* @param funEventCallback 事件回调 Event callback
* @param funDataCallback 数据回调 Data call back
* @param funDownloadDataCallback 下载数据回调 Download Data call back
* @param funRawDataCallback 解码后数据回调(Linux下此回调无效)  Data callback afer decoding(it is void by linux next time)
* @param funLanSearchDeviceCallback 设备检索回调 Search device callback
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_RegisterCallback(
	JCEventCallback_t funEventCallback,
	JCDataCallback_t funDataCallback,
	JCDownloadDataCallback_t funDownloadDataCallback,
	JCRawDataCallback_t funRawDataCallback,
	JCLanSearchDeviceCallback_t funLanSearchDeviceCallback);

//连接 connection-----------------------------------------------------------------------------------
/**
* @brief 建立连接 Build connection
* @param szServer 设备IP或云视通号码  Device IP/CLOUDSEE ID
* @param nPort 设备服务端口,bNumOrIP为TRUE是无效 For nPort device server port,if bNumberor IPC is true,that is void
* @param nChannel 设备通道号 Channel number
* @param szNetUser 用户名 user name
* @param szPwd 密码  passward
* @param bNumOrIP TRUE指明szServer为云视通号码,FALSE指明szServer为设备 True mean szserver is cloudseeid,false mean szserver is device
* @param pUserData 用户自定义数据，将会在回调中返回 User self-defining data datas wil be back in callback
* @return 成功调用返回连接ID，连接结果在事件回调中返回，调用失败返回JCSDK_INVALID_LINKVALUE 
*			success to callback to return connectoin ID,connection result will be return in event callback,Fail to call to return JCSDK_INVALID_LINKVALUE 
*/ 
JCSDK_API JCLink_t JCSDK_CALLTYPE JCSDK_Connect(char *szServer, int nPort, int nChannel,
	char *szNetUser, char *szPwd, JBOOL bNumOrIP, LPVOID pUserData);

/**
* @brief 断开连接  Connection stope
* @param nLinkID 连接ID  Connect ID
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_Disconnect(JCLink_t nLinkID);

/**
* @brief 获取设备信息 Gain device information
* @param nLinkID 连接ID Connect ID
* @param pDevInfo 设备信息结构体指针 Device inforamtion structure pointer
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_GetDeviceInfo(JCLink_t nLinkID, PJCDeviceInfo pDevInfo);

//解码预览 Decode preview-------------------------------------------------------------------------------
#ifndef LINUX
/**
* @brief 开启解码器(Linux下不支持此功能)  Start decoder(do not support under linux)
* @param nLinkID 连接ID   LinkID
* @param bEnable 是开启或关闭解码 Open or close decoding
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_EnableDecoder(JCLink_t nLinkID, JBOOL bEnable);

/**
* @brief 设置视频预览(Linux下不支持此功能) Set video preview(do not support under linux)
* @param nLinkID 连接ID   link ID
* @param hWnd 预览窗口句柄，关闭预览传NULL Preview window handle,close preview to send NULL
* @param pRect 预览区域，关闭预览传NULL Preview area,close preview to send NULL
* @return 返回调用结果 return back callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_SetVideoPreview(JCLink_t nLinkID, HWND hWnd, LPRECT pRect);

/**
* @brief 设置音频预览(Linux下不支持此功能) Set audio preview
* @param nLinkID 连接ID link ID
* @param hWnd 预览窗口句柄，关闭预览传NULL Preview window handle,close preview to send NULL
* @return 返回调用结果 Return to callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_SetAudioPreview(JCLink_t nLinkID, HWND hWnd);
#endif

/**
* @brief 开启音视频数据(默认状态是启用音视频) Open video and audio datas( default open)
* @param nLinkID 连接ID link ID
* @param bEnable 启用/禁用音视频数据流  Enable/diable video and audio data stream
* @return 返回调用结果 Return callback result
*/
//
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_EnableVideoData(JCLink_t nLinkID, JBOOL bEnable);

/**
* @brief 获取码流信息 Get stream information
* @param nLinkID 连接ID LINK ID
* @param pInfo 码流信息结果提指针 Stream information to get pointer
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_GetStreamInfo(JCLink_t nLinkID, PJCStreamInfo pInfo);

//录像功能-record function------------------------------------------------------------------------------
/**
* @brief 开启录像  Open record
* @param nLinkID 连接ID  Link ID
* @param szFilename 录像文件名 File name
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_StartRec(JCLink_t nLinkID, char *szFilename);

/**
* @brief 关闭录像 Close record
* @param nLinkID 连接ID link ID
* @return 返回调用结果 Return calback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_StopRec(JCLink_t nLinkID);

//远程操作 Remote operation-------------------------------------------------------------------------------
/**
* @brief 远程录像检索,查询结果通过事件回调返回 Remote search ,result will be return by event callback
* @param nLinkID 连接ID link ID
* @param pDate 查询时间段  search period
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_GetRemoteRecFileList(JCLink_t nLinkID, PJCDateBlock pDate);

/**
* @brief 播放远程录像  Play remote record
* @param nLinkID 连接ID  link ID
* @param nFileID 远程录像查询返回的文件 File was return by remote search
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_RemotePlay(JCLink_t nLinkID, int nFileID);

/**
* @brief 远程回访播放控制 Play control for remote playback
* @param nLinkID 连接ID link ID
* @param nCmd 控制命令 Control command
* @param nParam 控制参数 Control param
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_RemotePlayControl(JCLink_t nLinkID, JCRemotePlayCommand nCmd, int nParam);

#ifndef LINUX
/**
* @brief 刷新画面(播放暂停状态下刷新窗口需调用该接口重绘图像,Linux不支持该功能)
*			refresh image(refresh window to redraw image when play paused,linux do not support this function) 
* @param nLinkID 连接ID  link ID
* @return 返回调用结果  Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_Refresh(JCLink_t nLinkID);
#endif

/**
* @brief 远程录像下载,录像数据通过事件回调返回 Remote record download,record datas will be return by event callback
* @param nLinkID 连接ID link id
* @param nFileID 远程录像查询返回的文件ID  FILE ID was return by remote search
* @param szFilename 本地文件名  local filename
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_DownloadRemoteFile(JCLink_t nLinkID, int nFileID, const char* szFilename);

/**
* @brief 远程录像控制  Remote record control
* @param nLinkID 连接ID link ID
* @param bEnable 开启或关闭远程录像 Open/close remote record
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_RemoteRecording(JCLink_t nLinkID, JBOOL bEnable);

//语音对讲 Two way  talk-------------------------------------------------------------------------------
/**
* @brief 开启语音对讲,对方是否同意通过回调通知 Enable two way talk,agreed or not wil be return by callback information
* @param nLinkID 连接ID link ID
* @return 返回调用结果  return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_StartTalk(JCLink_t nLinkID);

/**
* @brief 停止语音对讲 Stop two way talk
* @param nLinkID 连接ID link ID
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_StopTalk(JCLink_t nLinkID);

/**
* @brief 发送对讲数据 Send datas of two way talk
* @param nLinkID 连接ID link ID
* @param pData 对讲数据指针 data point for two way talk
* @param nSize 对讲数据大小 data size fo two way talk
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_SendTalkData(JCLink_t nLinkID, PUCHAR pData, int nSize);

/**
* @brief 注册对讲回调函数 Registed callback function of two way talk
* @param nLinkID 连接ID link ID
* @param funTalkCallback 对讲数据回调函数 callback functin of two way talk datas
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_RegisterTalkCallback(JCTalkCallback_t funTalkCallback);

//其他 others-----------------------------------------------------------------------------------
/**
* @brief 局域网设备搜索,结果通过设备搜索回调返回 Search devices in LAN,result will be return by devices search callback
* @param nDeviceType 设备类型 DEVICE TYPE
* @param dwTimeout 毫秒超时时间 millisecond time out
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_LanSeartchDevice(int nDeviceType, DWORD dwTimeout);

#ifndef LINUX
/**
* @brief 截图(Linux下不支持此功能) Screencut(do not support under linux)
* @param nLinkID 连接ID link ID
* @param szFilename BMP图片文件名 BMP image name
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_SaveBitmap(JCLink_t nLinkID, char *szFilename);

/**
* @brief 远程设置(暂只支持IPC,Linux下不支持此功能)(即将废弃) Remote configuration( just support IPC,can not support under LINUX)( will be given up)
* @param nLinkID 连接ID link id
* @param nLanguage 0中文,1英文 0 chinese,1 english
* @return 返回调用结果 return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_RemoteConfig(JCLink_t nLinkID, int nLanguage);
#endif

/**
* @brief 发送云台命令 Send PTZ control command
* @param nLinkID 连接ID link id
* @param eCmd 云台命令 PTZ command
* @param bStartOrStop 启动或停止 start or stop
* @return 返回调用结果 return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_SendPTZCommand(JCLink_t nLinkID, JCPTZCmdType eCmd, JBOOL bStartOrStop);

/**
* @brief 色彩空间转换(yv12转rgb32)(行偏移:相邻两行行首之间地址偏移)color space conversion (change yv12 to rgb32)( line offset:Address offset between beginning of adjacent two lines
* @param pDest 目的缓存 aim cache
* @param dwDestPitch rgb32缓存行偏移 rgb cache line offset
* @param pSrcY 源Y数据区指针 pointer of Souce Y data area
* @param pSrcU 源U数据区指针 pointer of Souce u data area
* @param pSrcV 源V数据区指针 pointer of Souce v data area
* @param dwSrcYPitch 源Y数据区行偏移 offset of source Y data area
* @param dwSrcUPitch 源U数据区行偏移 offset of source U data area
* @param dwSrcVPitch 源v数据区行偏移 offset of source V data area
* @param dwWidth 图像宽度 image width
* @param dwHeight 图像高度 image height
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_yv12_to_rgb32(PUCHAR pDest, DWORD dwDestPitch, PUCHAR pSrcY, PUCHAR pSrcU, PUCHAR pSrcV, 
	DWORD dwSrcYPitch, DWORD dwSrcUPitch, DWORD dwSrcVPitch, DWORD dwWidth, DWORD dwHeight);

/**
* @brief 注册用户数据回调函数(保留功能) callback function for register user datas (save this function)
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_RegisterUserDataCallback(JCUserDataCallback_t funUserDataCallback);

/**
* @brief 注册用户数据回调函数(发送用户数据) callback function for register user datas (send user datas)
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_SendUserData(JCLink_t nLinkID, PUCHAR pData, int nSize);

/**
* @brief 开始GRPC远程设置(暂只新版本IPC支持,不支持返回FALSE) Start GRPC remote configuration (Only latest version IPC support, do not support FALSE)
* @param nLinkID 连接ID LINK ID
* @return 返回调用结果 return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_GRPC_Begin(JCLink_t nLinkID);

/**
* @brief 结束远程设置 end remote configuration
* @param nLinkID 连接ID link ID
* @return 返回调用结果 return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_GRPC_End(JCLink_t nLinkID);

//一键设置IP相关(现只有IPC支持) about one key to set IP(ONLY ipc support)----------------------------------------------------------
//设备信息 device information
typedef struct _JCOKIPDeviceInfo
{
	char szYSTNum[32];
	char szIP[16];
    int nPort;
	int nChannelNum;
    char szName[100];
	JCDeviceType dtType;
}
JCOKIPDeviceInfo, *PJCOKIPDeviceInfo;

//设备网络设置  device network setting
typedef struct _JCOKIPNetCfg
{
	JBOOL bDHCP;		//是否开启DHCP open DHCP or not
    DWORD dwIP;			//网络字节序IP地址 IP address of network byte order
    DWORD dwNetMask;	//网络字节序子网掩码  netmask of network byte order 
    DWORD dwGateWay;	//网络字节序网关 Gateway of network byte order 
    DWORD dwDNS;		//网络字节序DNS DNS of network byte order
}
JCOKIPNetCfg, *PJCOKIPNetCfg;

typedef void (JVSDK_CALLBACKTYPE *FunOKIPCallback_t)(PJCOKIPDeviceInfo pDev);

/**
* @brief 开启一键IP服务 Open one key to set IP
* @param funOKIPCallback 回调函数 callback function
* @return 返回调用结果 return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_OKIP_StartService(FunOKIPCallback_t funOKIPCallback);

/**
* @brief 停止一键IP服务 Stop one key to set IP
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_OKIP_StopService();

/**
* @brief 查找设备(异步调用，结果通过回调返回，pDev为NULL表示超时结束) Search device(asynchronous call,result wil be return by callback,pDev is null that mean timeout over)
* @param dwTimeout 毫秒超时时间(milliseconds timeout)
* @return 返回调用结果  return callback result
* @node 与JCSDK_LanSeartchDevice的不同：该方式通过广播方式实现，可以查找到局域网广播可到达的范围，可以检测到网络设置异常的设备。Node is different from JCSDK_LanSeartchDevice:It realize by broadcast,and search in area broadcasting covered,and can search abnormal devices
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_OKIP_FinDevs(DWORD dwTimeout);

/**
* @brief 设置设备网络参数 set device network parameters
* @param szYST 云视通号码 cloudsee id
* @param szUser 用户名 user name
* @param szPwd 密码 password
* @oaram pCfg 网络配置 Network configuration
* @return 返回调用结果 Return callback result
*/
JCSDK_API JBOOL JCSDK_CALLTYPE JCSDK_OKIP_SetNetCfg(char* szYST, char* szUser, char* szPwd, PJCOKIPNetCfg pCfg);

/**
* @brief 查询号码在线状态 get yst online status
* @param szYST 云视通号码 cloudsee id
* @param nTimeout 秒超时时间 timeout
* @return 在线返回正数，不在线返回0，查询失败返回负数
*/
JCSDK_API int JCSDK_CALLTYPE JCSDK_GetYstOnlineStatus(char* szYST, int nTimeout);

/**
* @brief 设置网卡 set netcard
* @param szCardName 网卡名字/物理地址 netcard name/mac addr (eth0,eth1,... for linux; mac addr for windows)
* @other 在JCSDK_InitSDK之前调用 call before JCSDK_InitSDK
*/
JCSDK_API void JCSDK_CALLTYPE JCSDK_SetNetCard(char* szCardName);
