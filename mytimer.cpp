#include"mytimer.h"

myTimer::myTimer(QObject *parent)
    :QTimer(parent)
{

}

myTimer::myTimer(LONG h, QObject *parent)
    :QTimer(parent),handle(h)
{
    connect(this,SIGNAL(timeout()),this,SLOT(finish()));
}

void myTimer::finish()
{
    emit timeOut(handle);
}
