#include "wrapper.h"
#include <QApplication>
#include<QFile>
#include"devpanel.h"
#include<QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QMessageBox>
#include<QString>
#include<QDebug>
#include<QObject>
#include"ruleWidget/rulewidget.h"
bool createConnection();
void initGlobData();
#include<QTranslator>
#include<QDebug>
#include<QDesktopWidget>
#include<QSettings>
#include<synchapi.h>
#include"preview/preview.h"
#include"customContro/switchbutton.h"
#include"globdata.h"
#include"z1801p.h"
#include"preview/ptzcontrol.h"
#include "preview/sqldata.h"
#include "singleapplication.h"
#include "logwidget.h"
#include "globaloption.h"
#include<QSplashScreen>
#include <QMetaType>

QApplication *gpMainApp = NULL;
QMap<int,AlarmType> typeMap = {};
QMap<int,QString> nameMap = {};
QMap<int,CameraDeviceImf> deviceImf = {};
DeviceTreeMode *treeModle = nullptr;
int page_max_count_record = 0;
QString glob_user = {};
QString glob_nickName = {};
int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv,"bjspld");
    //qRegisterMetaType<CameraDeviceImf>("CameraDeviceImf");
    if (a.isRunning())
    {
        a.sendMessage("message from other instance.");
        return 0;
    }
    if(!createConnection())
        return 1;
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("ACCOUNT_PASSWARD");
    bool isAutoLog = settings.value("IS_AUTO_LOG").toInt() == 1 ? true : false;
    int isCheck = settings.value("IS_SAVED").toInt();
    QString account = settings.value("ACCOUNT").toString();
    QString password = settings.value("PASSWARD").toString();
    settings.endGroup();
    settings.beginGroup("SAVE DIR");
    G_PicSaveAdd = settings.value("imageSaveDir").toString();
    G_VedioSaveAdd = settings.value("vedioSaveDir").toString();
    settings.endGroup();
    settings.beginGroup("SYS_EDIT");
    G_Port = settings.value("listenPort").toInt();
    G_RecordTime = settings.value("recordTime").toInt();
    G_Stream = settings.value("stream").toInt();
    settings.endGroup();
    logWidget *log = new logWidget;
    if(isCheck == 1)
    {
        log->update(true,isAutoLog,account,password);
    }
    if(log->exec() == QDialog::Rejected)
    {
        delete log;
        return 0;
    }
    delete log;
    glob_user = account;
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/image/splash.png").scaled(317,238));
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage("加载翻译文件",topRight,Qt::white);
    gpMainApp = &a;
    QTranslator tran;
    if(!tran.load("qt_zh_CN.qm",":\\"))
    {
        qDebug() << "加载翻译文件失败" << endl;
    }
    a.installTranslator(&tran);
    splash->showMessage("加载样式",topRight,Qt::white);
    QFile file(":/qss/CmenuBar.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    //typeMap = {{1,AlarmType::FIRE},{2,AlarmType::FIRE},{3,AlarmType::STONE_ALARM},{4,AlarmType::STONE_ALARM},{5,AlarmType::STONE_ALARM}};
    splash->showMessage("加载数据",topRight,Qt::white);
    initGlobData();
    SqlData *d = new SqlData();
    deviceImf = d->getCameraDeviceImf();

    //根据配置文件设置一些全局变量

    page_max_count_record = 50;
    splash->showMessage("正在启动。。。",topRight,Qt::white);
    Wrapper w;
    //SwitchButton w;
    QObject::connect(&a,SIGNAL(messageAvailable(QString)),&w,SLOT(receiveMessage(QString)));
    QDesktopWidget *desk=QApplication::desktop();
    int wd=desk->width();
    int ht=desk->height();
    w.move((wd-w.width())/2,(ht-w.height())/2);
    w.show();
    splash->finish(&w);
    delete  splash;
    return a.exec();
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("alarmImfor.db");
    if(!db.open())
    {
        QMessageBox::critical(0,QObject::tr("数据库连接错误"),db.lastError().text());
        return false;
    }
    return true;
}

void initGlobData()
{
    QSqlQuery query;
    query.exec("SELECT ALARM_NUM,ALARM_NAME FROM alarmImfor");
    while(query.next())
    {
        int num = query.value(0).toInt();
        QString name = query.value(1).toString();
        nameMap[num] = name;
    }
}

