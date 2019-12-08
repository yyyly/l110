#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include<QRgb>
#include<QString>

struct Ruler
{
    QRgb groundColor;
    QString soundAddress;
    bool isPlay;
    int playTimes;
};

#endif // CUSTOMDATA_H
