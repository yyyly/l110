#include "myedit.h"

myEdit::myEdit(QWidget *parent):
    QLineEdit(parent)
{

}

void myEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit(doubleClicked());
}
