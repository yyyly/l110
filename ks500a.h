#ifndef KS500A_H
#define KS500A_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "z1801.h"
#include "z1801p.h"

class Ks500a : public QObject
{
    Q_OBJECT
public:
    static Ks500a* get500a()
    {
        if(ks500a == nullptr)
        {
            ks500a = new Ks500a();
        }
        return ks500a;
    }
private:
    static Ks500a * ks500a;
    explicit Ks500a(QObject *parent = nullptr);
    QSerialPort *serial;
    QTimer *mTimer;
    QByteArray buffer;
    DeviceStatus statu;
    QTimer *timer;
    bool isReciviData;
public slots:
    void openSeria(const QString com);
    void closeSeria();
    void dataDisplay();
    void timeOut();
    void handleError(QSerialPort::SerialPortError err);
    void haveLongTimeNoData();
signals:
    void linkSucced(const QString message);
    void messing(AlarmMessing &messing);
    void login();
    void logout();
};



#endif // KS500A_H
