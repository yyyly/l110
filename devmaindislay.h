#ifndef DEVMAINDISLAY_H
#define DEVMAINDISLAY_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QPaintEvent>

#include "customContro/switchbutton.h"

class DocumetDisplay : public QTextEdit
{
    Q_OBJECT
public:
    DocumetDisplay(QWidget *parent = 0)
        : QTextEdit(parent)
    {}
};

class DevMainDislay : public QWidget
{
    Q_OBJECT
public:
    explicit DevMainDislay(QWidget *parent = 0);
private:
    DocumetDisplay *devStatus;
    QLabel *relayLabel_1;
    SwitchButton *relaySwitch_1;
    QLabel *relayLabel_2;
    SwitchButton *relaySwitch_2;
    QLabel *voice;
    SwitchButton *voiceSWitch;
    QPushButton *alarm;
    QPushButton *disAlarm;
    QPushButton *clearMark;
signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DEVMAINDISLAY_H
