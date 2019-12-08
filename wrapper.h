/*
 *功能：这个类为应用程序提供自定义显示外包装
*/
#ifndef WRAPPER_H
#define WRAPPER_H

#include <QWidget>

#include"cmenubar.h"
#include"devpanel.h"
#include"alarmwidget.h"
#include<QEvent>
#include<QPoint>
#include<QMoveEvent>
#include<QListView>
#include<QMap>
#include<QString>
#include<QVariant>
#include<QStringList>
#include<QPainter>
#include<QVector>
#include"preview/preview.h"
#include"playback/playback.h"
#include"messageRecord/messagerecord.h"
#include"editWidget/systemeditdialog.h"


class Wrapper : public QWidget
{
    Q_OBJECT
public:
    explicit Wrapper(QWidget *parent = 0);
    ~Wrapper();
private:
    //界面移动
    QPoint mousePos;
    bool mouseIsPress;
    void CustomDisplay();
    QVBoxLayout *VLayout;
    CMenuBar* menuBar;
    AlarmWidget* alarmWidget;
    MessageRecord *recordWidget;
    Preview* preview;
    PlayBack* playBack;
    QVector<QWidget *> PWidgetVec;
    SystemEditDialog *systemEidtDialog;
    int currentWidget;
signals:

public slots:
    void tableClicked(int n);
    void toMaxOrNormolSize();
    void toMinSize();
    void receiveMessage(const QString message);
    void aboutApp();
    void changeAccount();
    void manageAccount();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
};
#endif // WRAPPER_H
