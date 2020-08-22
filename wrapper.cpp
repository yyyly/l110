#include "wrapper.h"
#include<QMessageBox>
#include<QStyleOption>
#include<QAction>
#include<QSettings>
#include<QMessageBox>
#include<QSqlQuery>
#include<QScopedPointer>
#include<QApplication>
#include<QDesktopWidget>
#include"manageaccountdialog.h"
#include"changeaccountdidalog.h"
#include"aboutdialog.h"
#include"globdata.h"

Wrapper::Wrapper(QWidget *parent)
    : QWidget(parent),
      menuBar(new CMenuBar()),
      alarmWidget(new AlarmWidget()),
      preview(new Preview()),
      recordWidget(new MessageRecord()),
      playBack(new PlayBack()),
      VLayout(new QVBoxLayout()),
      systemEidtDialog(new SystemEditDialog())
{
    menuBar->installEventFilter(this);
    PWidgetVec.append(alarmWidget);
    PWidgetVec.append(preview);
    PWidgetVec.append(recordWidget);
    PWidgetVec.append(playBack);
    currentWidget = 0;
    setLayout(VLayout);
    setCursor(Qt::ArrowCursor);
    CustomDisplay();
}

Wrapper::~Wrapper()
{

}

void Wrapper::CustomDisplay()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    QRect rect = QApplication::desktop()->screenGeometry();
    setMinimumSize(QSize(rect.width()*0.90,rect.height()*0.90));
    VLayout->addWidget(menuBar);
    VLayout->addWidget(PWidgetVec[currentWidget]);
    VLayout->setMargin(0);
    VLayout->setSpacing(0);
    QAction *action = menuBar->addTitleAction("接警规则");
    action->setIcon(QIcon(":/image/guize.png"));
    QAction *editAction = menuBar->addTitleAction("系统设置");
    editAction->setIcon(QIcon(":/image/systemEdit.png"));
    QAction *messingAction = menuBar->addTitleAction("信息录入");
    messingAction->setIcon(QIcon(":/image/xinxiluru.png"));
    QAction *vedioLinkAction = menuBar->addTitleAction("联动配置");
    vedioLinkAction->setIcon(QIcon(":/image/liandon.png"));
    connect(action,SIGNAL(triggered()),alarmWidget->ruleDialog,SLOT(exec()));
    connect(editAction,SIGNAL(triggered()),systemEidtDialog,SLOT(exec()));
    connect(messingAction,SIGNAL(triggered()),alarmWidget->dataEntryDialog,SLOT(exec()));
    connect(vedioLinkAction,SIGNAL(triggered()),alarmWidget->vediolLinkConfigDialog,SLOT(exec()));
    connect(menuBar,SIGNAL(TableCliecked(int)),this,SLOT(tableClicked(int)));
    connect(menuBar,SIGNAL(toMaxSize()),this,SLOT(toMaxOrNormolSize()));
    connect(menuBar,SIGNAL(toMinSize()),this,SLOT(toMinSize()));
    connect(menuBar,&CMenuBar::about,this,&Wrapper::aboutApp);
    connect(menuBar,&CMenuBar::changeAccount,this,&Wrapper::changeAccount);
    connect(menuBar,&CMenuBar::manageAccount,this,&Wrapper::manageAccount);
}

bool Wrapper::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == menuBar && !this->isMaximized())
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
    }
    if(event->type() == QEvent::Close)
    {
        QMessageBox msgBox;
        msgBox.setText("关闭客户端");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
            close();
        else event->ignore();
    }
    return false;
}

void Wrapper::paintEvent(QPaintEvent *event)
{
    /*Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setPen(QPen(Qt::black,2));
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(1,1,width()-2,height()-2,0,0);*/
}


void Wrapper::receiveMessage(const QString message)
{

}
void Wrapper::tableClicked(int n)
{
    if(n == currentWidget)
        return;
    PWidgetVec[n]->show();
    VLayout->removeWidget(PWidgetVec[currentWidget]);
    PWidgetVec[currentWidget]->hide();

    VLayout->insertWidget(1,PWidgetVec[n]);
    currentWidget = n;
}

void Wrapper::toMaxOrNormolSize()
{
    if(this->isMaximized())
    {
      showNormal();
      menuBar->showNormal();
    }

    else
    {
        showMaximized();
        menuBar->showMax();
    }

}

void Wrapper::toMinSize()
{
    showMinimized();
}

void Wrapper::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    this->setAttribute(Qt::WA_Mapped);
}

void Wrapper::aboutApp()
{
    aboutDialog dialog;
    dialog.exec();
}

void Wrapper::changeAccount()
{
    ChangeAccountDidalog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        QString account = dialog.getCurrentAccount();
        menuBar->updateNickName(account);
        glob_user = account;
        QMessageBox::warning(this,"提示","账户切换成功");
    }
    else {
        //QMessageBox::warning(this,"提示","账户切换失败");
    }
}

void Wrapper::manageAccount()
{
    QScopedPointer<ManageAccountDialog> dialog(new ManageAccountDialog());
    dialog->exec();
}
