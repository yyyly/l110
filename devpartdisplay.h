#ifndef DEVPARTDISPLAY_H
#define DEVPARTDISPLAY_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QLabel>
#include<QStringList>
#include<QGridLayout>
#include<QVBoxLayout>
#include <QPainter>
#include <QPushButton>

enum IndicatorStatu
{
    ALARM,
    DISALARM,
    PASS
};

class Indicator : public QLabel
{
    Q_OBJECT
public:
    Indicator(const QString &n,IndicatorStatu s, QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
private:
    IndicatorStatu statu;
};
class DevPartDisplay : public QWidget
{
    Q_OBJECT
public:
    DevPartDisplay(QWidget *parent = 0);
    DevPartDisplay(const QString &list,QWidget *parent = 0);
    void partAreaChange(const QString &list);
    QWidget *widget;
private:
    QStringList alarmList;
    QVector<Indicator *> alarmIconList;
    QGridLayout *rLayout;
    QVBoxLayout *vLayout;
    QPushButton *alarmButton;
    QPushButton *disAlarmButton;
    Indicator  *alarmIndicator;
    Indicator *disAlarmIndictor;
    Indicator *passIndictor;
    int num;
    int p;//字符大小
signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DEVPARTDISPLAY_H
