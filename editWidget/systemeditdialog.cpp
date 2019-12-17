#include "systemeditdialog.h"
#include "ui_systemeditdialog.h"
#include"style/customtabwidgetstyle.h"
#include "globaloption.h"
#include<QFileDialog>
#include<QSettings>
#include<QMouseEvent>
#include<QPainter>
#include<QStyleOption>

SystemEditDialog::SystemEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemEditDialog)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->setupUi(this);
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(new CustomTabWidgetStyle);
    //ui->tabWidget->tabBar()->setTabEnabled(0,false);
    ui->widget->installEventFilter(this);
    ui->closeButton->setButtonStyle(":/image/toolbar_close.png",4);
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("SAVE DIR");
    ui->vedioDirEdit->setText(settings.value("vedioSaveDir").toString());
    ui->imageDirEdit->setText(settings.value("imageSaveDir").toString());
    G_PicSaveAdd = settings.value("imageSaveDir").toString();
    G_VedioSaveAdd = settings.value("vedioSaveDir").toString();
    settings.endGroup();
    defPicSaveDir = "D:/Pictures";
    defVedioSaveDir = "D:/Pictures";
    connect(ui->imageDirButton,SIGNAL(clicked()),this,SLOT(setImageSaveDir()));
    connect(ui->vedioDirButton,SIGNAL(clicked()),this,SLOT(setVedioSaveDir()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));

}

SystemEditDialog::~SystemEditDialog()
{
    delete ui;
}

void SystemEditDialog::setImageSaveDir()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("图片保存路径"),
                                                    G_PicSaveAdd,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->imageDirEdit->setText(dir);
    }
}

void SystemEditDialog::setVedioSaveDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("视频保存路径"),
                                                    G_VedioSaveAdd,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->vedioDirEdit->setText(dir);
    }
}

QString SystemEditDialog::getImageSaveDir()
{
    return ui->imageDirEdit->text();
}

QString SystemEditDialog::getVedioSaveDir()
{
    return ui->vedioDirEdit->text();
}

bool SystemEditDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        switch(mouseEvent->type())
        {
        case QEvent::MouseButtonPress:
            {
                if(mouseEvent->button() == Qt::LeftButton)
                {
                    mouseIsPress = true;
                    mousePos = mouseEvent->pos();
                }
            }
                break;
        case QEvent::MouseMove:
            {
                move(mouseEvent->pos() - mousePos  + pos());
            }
                break;
        case QEvent::MouseButtonRelease:
            {
                mouseIsPress = false;
            }
                break;
        }
        return false;
    }
    return false;
}

void SystemEditDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setPen(QPen(Qt::black,2));
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(1,1,width() -2,height()-2,0,0);
}

void SystemEditDialog::closeButtonClicked()
{
    this->close();
}

void SystemEditDialog::on_dirSaveButton_clicked()
{
    G_PicSaveAdd = getImageSaveDir();
    G_VedioSaveAdd = getVedioSaveDir();
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("SAVE DIR");
    settings.setValue("vedioSaveDir",getVedioSaveDir());
    settings.setValue("imageSaveDir",getImageSaveDir());
    settings.endGroup();
}

void SystemEditDialog::on_dirDefaultButton_clicked()
{
    ui->imageDirEdit->setText(defPicSaveDir);
    ui->vedioDirEdit->setText(defVedioSaveDir);
}
