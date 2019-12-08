#ifndef MYTIMER_H
#define MYTIMER_H

#include<QTimer>
#include"HCNetSDK.h"

class myTimer : public QTimer
{
    Q_OBJECT

public:
    explicit myTimer(QObject *parent = 0);
    explicit myTimer(LONG h,QObject *parent = 0);
private:
    LONG handle;
private slots:
    void finish();
signals:
    void timeOut(LONG handle);
};

#endif // MYTIMER_H
