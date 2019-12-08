#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QTreeView>
#include "screenbar.h"
#include "devicetreemode.h"
#include "ptzcontrol.h"
#include "sqldata.h"

class SerchEdit;
class QSortFilterProxyModel;
class  AddAndEditeDialog;

typedef struct DevConnectedID
{
    QString deviceID;
    int connectedID;
} DevConnectedID;

class Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget *parent = 0);
    void login(QMap<int,CameraDeviceImf> &imf);
private:
    QTreeView *deviceView;
    ScreenBar *screenBar;
    DeviceTreeMode *mode;
    PTZControl *ptzControl;
    CameroNet *hk;
    SerchEdit *serchEdit;
    QPushButton *addAndEditButton;
    QSortFilterProxyModel *proxyModel;
    AddAndEditeDialog *dialog;

signals:

public slots:
    void DeviceDoublePress(const QModelIndex &index);
    void playCamero(CameraDeviceImf *camero, Camero::LineState state);
    void ptz(Camero::PTZcommond c,int start);
    void setSerchReusl(const QString &text);
    void addAndEditButtonClicked();
protected:

};

#endif // PREVIEW_H
