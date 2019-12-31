#ifndef SCREENPOOLS_H
#define SCREENPOOLS_H

#include <QObject>
#include <QVector>

class Screen;

class ScreenPools
{
public:
    ScreenPools(QWidget *parent);
    ~ScreenPools();
    Screen * getSceen();
    int getPoolScreenNum(){return  screenVec.length();}
    QVector<Screen *> &getScreenVec(){return  screenVec;}
private:
    QVector<Screen *> screenVec;
    QWidget *parentWidget;
};

#endif // SCREENPOOLS_H
