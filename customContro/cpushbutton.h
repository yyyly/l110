#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H
#include<QPushButton>
#include<QString>

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
    CPushButton(QWidget *parent = 0);
    void setButtonStyle(QString imgsrc, int CutSec);
    //void resizeEvent(QResizeEvent *event);
};

#endif // CPUSHBUTTON_H
