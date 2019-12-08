#ifndef MYEDIT_H
#define MYEDIT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QMouseEvent>

class myEdit : public QLineEdit
{
    Q_OBJECT
public:
    myEdit(QWidget *parent);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event){event->ignore();}
signals:
    void doubleClicked();
};

#endif // MYEDIT_H
