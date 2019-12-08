#include "cmenubar.h"
#include "style/currentstyle.h"
#include<qstyleoption.h>
#include<QPainter>
#include<QPixmap>
#include<QImage>
#include<QBrush>
#include<QPalette>
#include<QSqlQuery>
#include<QMouseEvent>
#include "globdata.h"
CMenuBar::CMenuBar(QWidget *parent)
    : QWidget(parent),
      tabBar(new QTabBar(this)),
      Icon(new QMenuBar(this)),
      closeButton(new QPushButton(this)),
      toMaxButton(new QPushButton(this)),
      toMinButton(new QPushButton(this)),
      accountButton(new QPushButton(this)),
      userMenu(new QMenu(this)),
      nickNameLabel(new QLabel(this))
{
    updateNickName(glob_user);
    setFixedHeight(40);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setObjectName("CmenuBar");
    QHBoxLayout *midHLayout = new QHBoxLayout(this);
    midHLayout->setContentsMargins(12,0,0,0);
    midHLayout->setSpacing(12);
    tabBar->addTab("报警主机");
    tabBar->addTab("视频预览");
    tabBar->addTab("记录查询");
    Icon->setFixedSize(40,40);
    QStyle *LocalStyle = Icon->style();
    CurrentStyle *mystyle = new CurrentStyle();
    Icon->setStyle(mystyle);
    menu = Icon->addMenu(QIcon(":/image/icon.ico"),"");
    menu->setStyle(LocalStyle);
    QAction* changeAccountAction = userMenu->addAction("切换账户");
    //QAction* manageAccountAction = userMenu->addAction("管理账户");
    QAction* aboutAction = userMenu->addAction("关于");
    //closeButton->setFixedSize(18,16);
    //toMinButton->setFixedSize(18,16);
    //toMaxButton->setFixedSize(18,16);
    //closeButton->setObjectName("closeButton");
    //toMinButton->setObjectName("toMinButton");
    //toMaxButton->setObjectName("toMaxButton");
    setButtonStyle(closeButton,":/image/toolbar_close.png",4);
    setButtonStyle(toMaxButton,":/image/toolbar_max.png",4);
    setButtonStyle(toMinButton,":/image/toolbar_min.png",4);
    setButtonStyle(accountButton,":/image/toolbar_User.png",4);
    //accountButton->setMenu(menu);
    closeButton->setToolTip("关闭");
    toMinButton->setToolTip("最小化");
    toMaxButton->setToolTip("最大化");
    nickNameLabel->setStyleSheet("QLabel{font: 11pt '隶书';color:white;text-overflow: ellipsis;}");
    nickNameLabel->setFixedWidth(45);
    QHBoxLayout *rhLayout = new QHBoxLayout();
    QHBoxLayout *rrhLayout = new QHBoxLayout();
    rhLayout->setMargin(10);
    rhLayout->setSpacing(12);
    rrhLayout->setMargin(0);
    rrhLayout->setSpacing(6);
    rrhLayout->addWidget(accountButton);
    rrhLayout->addWidget(nickNameLabel);
    rhLayout->addLayout(rrhLayout);
    rhLayout->addSpacing(12);
    rhLayout->addWidget(toMinButton);
    rhLayout->addWidget(toMaxButton);
    rhLayout->addWidget(closeButton);
    rhLayout->setStretch(1,12);
    QVBoxLayout *rVLayout = new QVBoxLayout(this);
    rVLayout->addStretch();
    rVLayout->addLayout(rhLayout);
    rVLayout->addStretch();
    midHLayout->addWidget(Icon);
    midHLayout->addWidget(tabBar);
    midHLayout->addStretch();
    midHLayout->addLayout(rVLayout);
    nickNameLabel->installEventFilter(this);
    connect(changeAccountAction,&QAction::triggered,this,&CMenuBar::ChangeAccountSlot);
    //connect(manageAccountAction,&QAction::trigger,this,&CMenuBar::manageAccountSlot);
    connect(aboutAction,&QAction::triggered,this,&CMenuBar::aboutSlot);
    connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(toMaxButton,SIGNAL(clicked(bool)),this,SLOT(toMaxButtonClicked()));
    connect(toMinButton,SIGNAL(clicked(bool)),this,SLOT(toMinButtonClicked()));
    connect(tabBar,SIGNAL(tabBarClicked(int)),this,SLOT(tableClicked(int)));
    connect(accountButton,SIGNAL(clicked()),this,SLOT(accountButtonClicked()));
}

void CMenuBar::updateNickName(const QString &name)
{
    QSqlQuery query;
    query.exec(QString("SELECT NICKNAME FROM logImfor WHERE ACCOUNT = '%1'").arg(name));
    if(query.next())
    {
        glob_nickName = query.value(0).toString();
    }
    nickNameLabel->setText(glob_nickName);
}

void CMenuBar::ChangeAccountSlot()
{
    emit changeAccount();
}

void CMenuBar::manageAccountSlot()
{
    emit manageAccount();
}

void CMenuBar::aboutSlot()
{
    emit about();
}

void CMenuBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QAction* CMenuBar::addTitleAction(const QString &str)
{
    QAction *action = menu->addAction(str);
    return action;
}

void CMenuBar::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void CMenuBar::tableClicked(int n)
{
    emit TableCliecked(n);
}

void CMenuBar::toMaxButtonClicked()
{
    emit toMaxSize();
}

void CMenuBar::toMinButtonClicked()
{
    emit toMinSize();
}

void CMenuBar::accountButtonClicked()
{
    userMenu->exec(accountButton->mapToGlobal(QPoint(-(menu->width() - accountButton->width() - nickNameLabel->width()) / 2,20)));
}

void CMenuBar::showMax()
{
    setButtonStyle(toMaxButton,":/image/toolbar_normal.png",4);

    toMaxButton->setToolTip("向下还原");
}

void CMenuBar::showNormal()
{
    setButtonStyle(toMaxButton,":/image/toolbar_max.png",4);
    toMaxButton->setToolTip("最大化");
}

void CMenuBar::setButtonStyle(QPushButton *button, QString imgsrc, int CutSec)
{
    int img_w=QPixmap(imgsrc).width();
    int img_h=QPixmap(imgsrc).height();
    int PicWidth = img_w/CutSec;
    button->setFixedSize(PicWidth,img_h);
    button->setStyleSheet(QString("QPushButton{border-width: 41px; border-image: url(%1) 0 0 0 %2 repeat repeat;border-width: 0px; border-radius: 0px;}")
    .append("QPushButton::hover{border-image: url(%1) 0 0 0 %3 repeat repeat;}")
    .append("QPushButton::pressed{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
    .append("QPushButton::checked{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
    .append("QPushButton::disabled{border-image: url(%1) 0 0 0 %5 repeat repeat;}")
    .append("QPushButton::menu-indicator{image:none;}")
    .arg(imgsrc).arg(0).arg(PicWidth*1).arg(PicWidth*2).arg(PicWidth*3));
}

bool CMenuBar::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == nickNameLabel)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->type() == QMouseEvent::MouseButtonPress)
        {
            userMenu->exec(accountButton->mapToGlobal(QPoint(-(menu->width() - accountButton->width() - nickNameLabel->width()) / 2,20)));
            return true;
        }
    }
    else {
        return false;
    }
}
