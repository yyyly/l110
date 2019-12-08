#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H

#include <QWidget>
#include<QTableView>
#include<QTreeView>
#include<QSqlQuery>
#include<QSqlTableModel>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QGraphicsRectItem>
#include<QStatusBar>
#include<QSortFilterProxyModel>
#include<QSound>
#include"z1801.h"
#include"csqltablemode.h"
#include "ruleWidget/rulewidget.h"
#include "areamap.h"
#include "devicemodel/devicemodel.h"
#include "devicemodel/imforparse.h"
#include "devicemodel/node.h"
#include "devicemodel/nodedelegate.h"
#include "contralpanel.h"
#include "editWidget/devicesetwidget.h"
#include "dataEntry/dataentrydialog.h"
#include "VedioLink/vediolinkconfigdialog.h"
#include "controlwidget.h"

struct RelatedMessingByNum
{
    QString name;
    QString person_1_name;
    QString person_1_phone;
    QString person_2_name;
    QString person_2_phone;
};

class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = 0);
    void updateMessing();
private:
    ContralPanel *contralPanel;
    QTreeView *devPanel;
    QTableView *imforView;
    ControlWidget *controWidget;
    QMap<QString,QVariant> partImfor[9];
    QStringList numberList;
    AreaMap *areaMap;
    AlarmDeviceImf alarmDeviceImf;
    CSqlTableMode *model;
    QSortFilterProxyModel *proxyModel;
    DeviceModel *devModel;
    Z1801 *z1801;
    DeviceSetWidget *deviceSetWidget;
    QMap<QString,Ruler> rulers;
    QStatusBar *statusBar;
    //从数据库中读取防区的名称
    QMap<int,RelatedMessingByNum> alarmMessingMap;
    QMap<int,RelatedMessingByNum> partMessingMap;
    QMap<int,QString> messingTypeMap;
    void InitAlarmDeviceImf();
    int currentAlarmNum;
    QSound *soundPlayer;

public:
    RuleWidget *ruleDialog;
    DataEntryDialog *dataEntryDialog;
    VedioLinkConfigDialog *vediolLinkConfigDialog;
signals:
    void DataPush(MessingType type,QMap<QString,QVariant> *data);
    void alarmNumCheck(int n);
public slots:
    void rulerChanged();

    void alarmDviceImf(AlarmStatu* statuArray,int* PartArray);

    void alarmMessing(AlarmMessing messing);

    void alarmNumFromView(QModelIndex index);

    void messingFilter(int state);// 0 只显示报警信息，1只显示系统信息，2显示所有信息

    void stopRecord(LONG handle);

    void deviceLog();

    void deviceDefence();

    void deviceDisDefence();

    void updatePartImfor(const PartImf &imf);

    void showBarMessing(const QString &messing);

    void updateNickNmae(const QString &name);
};

#endif // ALARMWIDGET_H
