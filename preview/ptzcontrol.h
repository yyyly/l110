#ifndef PTZCONTROL_H
#define PTZCONTROL_H

#include <QWidget>
#include <QPaintEvent>
#include "enums.h"

namespace Ui {
class PTZControl;
}



class PTZControl : public QWidget
{
    Q_OBJECT

public:
    explicit PTZControl(QWidget *parent = 0);
    ~PTZControl();

private:
    Ui::PTZControl *ui;
protected:
    void paintEvent(QPaintEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void toDerection(Camero::PTZcommond c,int start);
};

#endif // PTZCONTROL_H
