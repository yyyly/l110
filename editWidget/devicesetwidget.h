#ifndef DEVICESETWIDGET_H
#define DEVICESETWIDGET_H

#include <QDialog>
#include "devicesetmodel.h"
#include "z1801.h"
#include "cstatubar.h"

namespace Ui {
class DeviceSetWidget;
}

class DeviceSetWidget : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceSetWidget(QWidget *parent = nullptr,Z1801 *z = nullptr);
    ~DeviceSetWidget();
    DeviceSetModel * getModel() {return model;}
    int getStartAddress();

private slots:
    void newAlarmPropertys(QList<AlarmProperty> propertyList);

    void on_pollButton_clicked();

    void on_setButton_clicked();

    void on_busRadioButton_clicked(bool checked);

    void on_wirelessRadioButton_clicked(bool checked);

    void on_wireRadioButton_clicked(bool checked);

    void on_partNumInquireButton_clicked();

    void partNumUpdate(int c);

    void on_partNumEditButton_clicked();

    void on_timeInquirePushButton_clicked();

    void on_timeEditPushButton_clicked();

    void delayTimeUpdate(const deviceDelayTime &t);

    void on_remoterLermButton_clicked();

    void on_detecterLermButton_clicked();

    void on_defenceTimeInquirePushButton_clicked();
    
    void defenceTimeUpdate(const autoDefenceImfor &imf);

    void on_defenceTimeEditPushTime_clicked();

    void on_setDeviceTimepushButton_clicked();

    void on_keyInquirepushButton_clicked();

    void on_keyEditpushButton_clicked();

    void updateKeyMessing(const KeyOption &option);

private:
    Ui::DeviceSetWidget *ui;
    DeviceSetModel *model;
    Z1801 *z1801;
    bool isSetReplay;
    int replayNum;
    CStatuBar *bar;
    KeyOption keyOption;
};

#endif // DEVICESETWIDGET_H
