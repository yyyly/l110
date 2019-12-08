#include "screenbar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QEvent>
#include <cmath>

#define SCREEN_MAX_NUM 16

ScreenBar::ScreenBar(int n,QWidget *parent)
    :QWidget(parent),
      gLayout(new QGridLayout()),
      n(n),
      bottom(new BottomBar)
{
    Screen *screen = nullptr;
    for(int i = 0;i < n;i++)
    {
        screen = new Screen(i);
        connect(screen,SIGNAL(selectedStateIsChange(int)),this,SLOT(selectScreenChanged(int)));
        pScreenVec.push_back(screen);
    }
    pScreenVec.value(0)->setSelectState(Screen::SELECTED);
    selectedScreen = pScreenVec.value(0);
    previousSelectedScreen = nullptr;
    gLayout->setMargin(2);
    gLayout->setSpacing(2);
    setCustomLayout(n,0);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addLayout(gLayout);
    vLayout->addWidget(bottom);
    setLayout(vLayout);
    connect(bottom,SIGNAL(screenDisplayNumChanged(int)),this,SLOT(changeScreenNum(int)));
}


void ScreenBar::setCustomLayout(int screenNum,int type)
{
    Q_UNUSED(type);
    if(screenNum == 2)
    {
        int i;
        Screen *screen;
        for(i = 0;i < screenNum;i++)
        {
            screen = pScreenVec.value(i);
            screen->show();
           gLayout->addWidget(screen,0,i,1,1);
        }
    }
    else {
        int rowNum = (int)std::sqrt(screenNum);
        int i;
        Screen *screen;
        for(i = 0;i < screenNum;i++)
        {
            screen = pScreenVec.value(i);
            screen->show();
           gLayout->addWidget(screen,i/ rowNum,i % rowNum,1,1);
        }
    }

}

void ScreenBar::changeScreenNum(int n)// n = 1,4,16
{
    Screen *screen;
    foreach (screen, pScreenVec) {
        screen->hide();
        gLayout->removeWidget(screen);
    }
    if(n > pScreenVec.length())
    {

        int i = pScreenVec.length();
        for(i;i < n;i++)
        {
            screen = new Screen(i);
            connect(screen,SIGNAL(selectedStateIsChange(int)),this,SLOT(selectScreenChanged(int)));
            pScreenVec.push_back(screen);
        }
    }
    setCustomLayout(n,0);
}

void ScreenBar::selectScreenChanged(int n)
{
    selectedScreen->setSelectState(Screen::UNSELECTED);
    selectedScreen = pScreenVec.value(n);
    selectedScreen->setSelectState(Screen::SELECTED);
}

Screen* ScreenBar::getScreen()
{
    return previousSelectedScreen;
}

void ScreenBar::setNextSelectScreen()
{
    int n = selectedScreen->getNum();
    selectedScreen->setSelectState(Screen::UNSELECTED);
    previousSelectedScreen = selectedScreen;
    int m = n + 1;
    if(m > 0 && m < pScreenVec.size() && pScreenVec.size() != SCREEN_MAX_NUM)
    {
        selectedScreen = pScreenVec.value(m);
        selectedScreen->setSelectState(Screen::SELECTED);
    }
    else {
        //自动到更多视频的布局
    }

}

QVector<Screen *>* ScreenBar::getScreens()
{
    return &pScreenVec;
}

void ScreenBar::setCameroNet(CameroNet *c)
{
    cameroNet = c;
}

void ScreenBar::setBottonBarVisibal(bool enable)
{
    bottom->setVisible(enable);
}

BottomBar::BottomBar(QWidget *parent)
    :QWidget(parent),
      signalScreen(new QPushButton),
      fourScreen(new QPushButton),
      sixteenScreen(new QPushButton)
{
    setFixedHeight(50);
    //signalScreen->setFixedSize(20,20);
    //fourScreen->setFixedSize(20,20);
    //sixteenScreen->setFixedSize(20,20);
    setButtonStyle(signalScreen,QString(":/image/common/1.png"),5);
    setButtonStyle(fourScreen,QString(":/image/common/4.png"),5);
    setButtonStyle(sixteenScreen,QString(":/image/common/16.png"),5);
    QHBoxLayout *rhLayout = new QHBoxLayout;
    rhLayout->setContentsMargins(0,0,10,0);
    rhLayout->addWidget(signalScreen);
    rhLayout->addWidget(fourScreen);
    rhLayout->addWidget(sixteenScreen);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addLayout(rhLayout);
    setLayout(mainLayout);
    signalScreen->installEventFilter(this);
    fourScreen->installEventFilter(this);
    sixteenScreen->installEventFilter(this);
}

void BottomBar::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BottomBar::setButtonStyle(QPushButton *button, QString imgsrc, int CutSec)
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
    .arg(imgsrc).arg(0).arg(PicWidth*1).arg(PicWidth*2).arg(PicWidth*3));
}

bool BottomBar::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(watched == signalScreen)
            emit screenDisplayNumChanged(1);
        else if(watched == fourScreen)
            emit screenDisplayNumChanged(4);
        else
            emit screenDisplayNumChanged(16);
        return true;
    }
    return false;

}


