#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>
#include "cameronet_global.h"

class  CAMERONETSHARED_EXPORT Camero : public QObject
{
    Q_OBJECT
public:
    explicit Camero(QObject *parent = nullptr);
    enum Mold
    {
        HK,
        DH
    };

    enum TYPE
    {
        NVR,
        IPC
    };

};

#endif // ENUMS_H
