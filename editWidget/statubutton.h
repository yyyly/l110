#ifndef STATUBUTTON_H
#define STATUBUTTON_H

#include <QObject>
#include <QPushButton>

class StatuButton : public QPushButton
{
    Q_OBJECT
public:
    StatuButton(QWidget *parent = nullptr);
    void SetState(int s);
    int getState(){return state;}
protected:
    void paintEvent(QPaintEvent *event);
private:
    int state;
private slots:
    void updateState();
};

#endif // STATUBUTTON_H
