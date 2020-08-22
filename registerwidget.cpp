#include "registerwidget.h"
#include "ui_registerwidget.h"
#include<QProcess>
#include <windows.h>
#include <tchar.h>
#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QSqlQuery>
#include <QClipboard>
#include <QMessageBox>
#include <QDateTime>
#include "globdata.h"
#include "z1801.h"

RegisterWidget::RegisterWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("注册");
    ui->lineEdit->setText(format_HASHString(hash_Encryption(get_SerialNumber())));
    ui->lineEdit->setReadOnly(true);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}


QString RegisterWidget::get_SerialNumber()
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

QString RegisterWidget::hash_Encryption(const QString temp)
{
    QByteArray byte_array;
    byte_array.append(temp);
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array,QCryptographicHash::Md5);
    return hash_byte_array.toHex().toUpper();
}

QString RegisterWidget::format_HASHString(const QString hashtemp)
{
    QString retemp = "";
    for(int i = 0; i < 7; i++)
    {
        retemp += hashtemp.mid(4*i,4) + "-";
    }
    retemp += hashtemp.mid(28,4);
    return retemp;
}

void RegisterWidget::on_registerButton_clicked()
{
    QString strOn64base = ui->textEdit->toPlainText();
    QByteArray arr;
    arr.append(strOn64base);
    QByteArray regStr = arr.fromBase64(arr);
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(regStr,&error);
    if(!(error.error == QJsonParseError::NoError))
    {
        QMessageBox::warning(this,"提醒","注册码格式错误");
        return;
    }
    QJsonObject jsonObject = document.object();
    int sig = jsonObject.value("sig").toInt();
    QString macCode = jsonObject.value("macCode").toString();
    QString time = QDateTime::currentDateTime().toString("dd.MM.yyyy");
    bool isTrail = jsonObject.value("trial").toBool();
    QString sql;
    QSqlQuery query;
    if(macCode != ui->lineEdit->text())
    {
        QMessageBox::warning(this,"注意","机器码错误");
        return;
    }
    sql = QString("SELECT COUNT() from register");
    query.exec(sql);
    if(query.next())
    {
        int n = query.value(0).toInt();
        if(n == 0)
        {
            sql = QString("INSERT INTO register (serialNumber,istrail,deviceType,regTime)"
                          " VALUES (\"%1\",%2,%3,\"%4\")").arg(macCode).arg(isTrail).arg(sig).arg(time);
        }
        else
        {
            sql = QString("UPDATE register SET serialNumber = \"%1\",istrail = %2 "
                          ",deviceType = %3 ,regTime = \"%4\"").arg(macCode).arg(isTrail).arg(sig).arg(time);
        }
    }
    qDebug() << sql << endl;
    query.exec(sql);
    accept();

}

void RegisterWidget::on_copyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lineEdit->text());
}
