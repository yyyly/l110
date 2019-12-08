#ifndef SCREENBAR_H
#define SCREENBAR_H

#include <QWidget>
#include <QVector>
#include<QGridLayout>
#include<QPushButton>
#include"screen.h"
#include"cameronet.h"

class BottomBar : public QWidget
{
    Q_OBJECT
public:
    BottomBar(QWidget *parent = 0);
private:
    QPushButton *signalScreen;
    QPushButton *fourScreen;
    QPushButton *sixteenScreen;

    void setButtonStyle(QPushButton *button, QString imgsrc, int CutSec);
signals:
    void screenDisplayNumChanged(int n);
protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
};


class ScreenBar : public QWidget
{
    Q_OBJECT
public:

    explicit ScreenBar(int n = 1,QWidget *parent = 0);
    void setCustomLayout(int screenNum,int type);
    Screen* getScreen();
    Screen* getSelectedScreen(){return selectedScreen;}
    void setNextSelectScreen();
    QVector<Screen *>* getScreens();
    void setCameroNet(CameroNet *c);
    void setBottonBarVisibal(bool enable);
private:
    QVector<Screen *> pScreenVec;
    QGridLayout *gLayout;
    int n;//n为播放界面个数
    Screen *selectedScreen;
    Screen *previousSelectedScreen;
    BottomBar *bottom;
    CameroNet *cameroNet;
signals:

public slots:
    void selectScreenChanged(int n);
    void changeScreenNum(int n);
};

#endif // SCREENBAR_H
