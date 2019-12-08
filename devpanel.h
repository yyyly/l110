#ifndef DEVPANEL_H
#define DEVPANEL_H

#include <QWidget>
#include<QLabel>
#include<QPushButton>
#include<QFrame>
#include<QMap>
#include<QVariant>
#include<QHBoxLayout>
#include<QVBoxLayout>

#include"devmaindislay.h"
#include"devpartdisplay.h"
#include"z1801.h"
class DevPanel : public QFrame
{
    Q_OBJECT
public:
    explicit DevPanel(QMap<QString,QVariant> *data,QWidget *parent = 0);

signals:

public slots:
    void leftButtonClick();
    void rightButtonClick();
    void haveNewData(MessingType type,QMap<QString,QVariant> *data);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QLabel *title;
    QLabel *DevStatu;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QWidget *centroWidget[9];
    QWidget *lastWidget = nullptr;
    QWidget *currentWidget;
    QMap<QString, QVariant> *d;
    DevMainDislay *mainDisplay;
    DevPartDisplay *partDisplay;
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    int index;
public:

    void initInterface();
};

#endif // DEVPANEL_H
