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
#include "registerwidget.h"
#include<QSplashScreen>
#include <QMetaType>
#include <QMutex>

#include<QProcess>
#include <windows.h>
#include <tchar.h>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDate>
#include <QSqlQuery>
#include <QMainWindow>
#include "log.h"


bool createConnection();
void initGlobData();
const QString get_serialNumber();
QString hash_Encryption(const QString temp);
QString format_HASHString(const QString hashtemp);

QApplication *gpMainApp = NULL;
QMap<int,AlarmType> typeMap = {};
QMap<int,QString> nameMap = {};
QMap<int,CameraDeviceImf> deviceImf = {};
DeviceTreeMode *treeModle = nullptr;
int page_max_count_record = 0;
QString glob_user = {};
QString glob_nickName = {};
MachineType mType = MachineType::KS_500A;

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)

{

    static QMutex mutex;

    mutex.lock();



    QString text;

    switch(type)

    {

    case QtDebugMsg:

        text = QString("Debug:");

        break;



    case QtWarningMsg:

        text = QString("Warning:");

        break;



    case QtCriticalMsg:

        text = QString("Critical:");

        break;



    case QtFatalMsg:

        text = QString("Fatal:");

    }



    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);

    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    QString current_date = QString("(%1)").arg(current_date_time);

    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);



    if(type == QtWarningMsg)
    {
        QFile file("log.txt");

        file.open(QIODevice::WriteOnly | QIODevice::Append);

        QTextStream text_stream(&file);

        text_stream << message << "rn";

        file.flush();

        file.close();

    }


    mutex.unlock();

}

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

    //检测软件是否注册
    QSqlQuery query;
    query.exec("SELECT * FROM register");
    QString serialNumber;
    bool isTrail;
    QString regTime;
    if(query.next())
    {
        serialNumber = query.value(0).toString();
        mType = (MachineType)query.value("deviceType").toInt();
        isTrail = query.value("istrail").toBool();
        regTime = query.value("regTime").toString();
    }

    QString loaclSerialNumber = format_HASHString(hash_Encryption(get_serialNumber()));
    if(serialNumber != loaclSerialNumber)
    {
        RegisterWidget rw;
        if(rw.exec() == QDialog::Rejected)
        {
            return 0;
        }
        else
        {
            qApp->quit();
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        }
    }
    if(isTrail)
    {
        QDate regdate = QDate::fromString(regTime,"dd.MM.yyyy");
        QDate currentDate = QDate::currentDate();
        qint64 d = regdate.daysTo(currentDate);
        if(d >=10)
        {
            RegisterWidget rw;
            if(rw.exec() == QDialog::Rejected)
            {
                return 0;
            }
            else
            {
                qApp->quit();
                QProcess::startDetached(qApp->applicationFilePath(), QStringList());
            }
        }

    }
    Log log;
    log.initLog();
    QSettings settings("./settings.ini",QSettings::IniFormat);
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
    logWidget *login = new logWidget;
    if(isCheck == 1)
    {
        login->update(true,isAutoLog,account,password);
    }
    if(login->exec() == QDialog::Rejected)
    {
        delete login;
        return 0;
    }
    glob_user = login->getAccont();
    if(glob_user != account)
    {
        settings.beginGroup("ACCOUNT_PASSWARD");
        settings.setValue("ACCOUNT",glob_user);
        settings.setValue("PASSWARD",login->getPassword());
        settings.endGroup();
    }
    delete login;
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

    page_max_count_record = 5000;
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

const QString get_serialNumber()
{

    QString cpu_id = "";
        QProcess p(0);
        p.start("wmic CPU get ProcessorID");
        p.waitForStarted();
        p.waitForFinished();
        cpu_id = QString::fromLocal8Bit(p.readAllStandardOutput());
        cpu_id = cpu_id.remove("ProcessorId").trimmed();

        QString lpRootPathName = "C:\\";
        LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
        DWORD nVolumeNameSize=12;// 卷标的字符串长度
        DWORD VolumeSerialNumber;//硬盘序列号
        DWORD MaximumComponentLength;// 最大的文件长度
        LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
        DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
        DWORD FileSystemFlags;// 文件系统的一此标志
        GetVolumeInformation((LPTSTR)lpRootPathName.utf16(),
                             lpVolumeNameBuffer, nVolumeNameSize,
                             &VolumeSerialNumber, &MaximumComponentLength,
                             &FileSystemFlags,
                             lpFileSystemNameBuffer, nFileSystemNameSize);
        return (cpu_id.mid(0,4) + "D-"+ cpu_id.mid(4,4) + "R-" +
                cpu_id.mid(8,4) + "E-" + cpu_id.mid(12,4) + "A-" +
                QString::number(VolumeSerialNumber,10).mid(0,4)+"M");
}

QString hash_Encryption(const QString temp)
{
    QByteArray byte_array;
        byte_array.append(temp);
        QByteArray hash_byte_array = QCryptographicHash::hash(byte_array,QCryptographicHash::Md5);
        return hash_byte_array.toHex().toUpper();
}

QString format_HASHString(const QString hashtemp)
{
    QString retemp = "";
        for(int i = 0; i < 7; i++)
        {
            retemp += hashtemp.mid(4*i,4) + "-";
        }
        retemp += hashtemp.mid(28,4);
        return retemp;
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

