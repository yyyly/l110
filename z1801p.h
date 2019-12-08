#ifndef Z1801P_H
#define Z1801P_H

enum DeviceStatus
{
    Logined,
    DisLogin,
    Enale,
    DisEnale
};
enum AlarmStatu
{
    Defence,
    OutDefence,
    ByPass,
    FAULT,
    WARMED,
    UN_USED,
    NoStaru
};

enum ZoneStatu
{
    DefReady,
    DefNotReady,
    OutDefReady,
    OutDefNotReady
};
enum MessingType
{
    WARMING, //报警信息
    Login,
    HeartBeat,
    NONE
};

enum AlarmType
{
    ALARM_TYPE_ALL = -1,
    ALLDAY_ALARM_NOVEDIO = 0x1122,//24小时无声报警
    ALLDAY_ALARM_VEDIO = 0x1123,//24小时有声报警
    FIRE = 0x1110,//火警报警
    AIR_ALARM = 0x1151,//气感报警
    STONE_ALARM = 0x1130,//防盗报警
    REMOTE_DEFENCE = 0x3407,
    REMOTE_OUTDEFENCE = 0x1407
};

enum DeviceType
{
    SYSTEM,//系统
    USER,//用户
    ZONE,//防区
    KEY,//键盘
    MODULE//模块
};

enum PartStatus
{
    ALARMING,
    DISALARMING,
    READY,
    NOREADY
};

enum SystemStatus
{
    NOCONFLICT = 0,
    ACFALUT = 1,
    BATTERYLOW = 2,
    AREADDRESCONFLICT = 3,
    KEYADDRESCONFLICT = 4,
    ADDRESCONFLICT = 5
};

enum PartType
{
    FAULT_MESSING = 0x07,//故障信息
    BYPASS_MESSING = 0x08,//盘路信息
    WARMING_MESSING = 0x09,//报警信息
    LOW_BATTERY_MESSING = 0x0a,//低电信息
    DEFENCE_MESSING = 0x0b,//布防信息
    USED_ALARM_MESSING = 0x0c,//已使用的防区
    AREA_MESSING = 0x06,//分区包含防区信息
 };

enum AlarmAddress
{
    WRIED_ALARM_L_ADDRESS = 0x0700,
    WRIED_ALARM_H_ADDRESS = 0x071e,
    BUS_ALARM_L_ADDRESS = 0x0720,
    BUS_ALARM_H_ADDRESS = 0x08fe,
    WIRELESS_ALARM_L_ADDRESS = 0x0900,
    WIRELESS_ALARM_H_ADDRESS = 0x09fe
};

enum ModuleType
{
    Z861A,
    Z851A
};

#endif // Z1801P_H
