#ifndef BOMBSCREEN_H
#define BOMBSCREEN_H

#include <QDialog>
class screenBar;

struct AlarmImforOnBombScreen
{
    int alarmNum;
    QString alarmName;
    QString charge1Name;
    QString charge1Phone;
    QString charge2Nmae;
    QString chage2Phone;
};

namespace Ui {
class BombScreen;
}

class BombScreen : public QDialog
{
    Q_OBJECT

public:
    explicit BombScreen(AlarmImforOnBombScreen imfor,QWidget *parent = nullptr);
    ~BombScreen();

private:
    Ui::BombScreen *ui;
    int alarm_num;
};

#endif // BOMBSCREEN_H
