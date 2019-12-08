#include "cpushbutton.h"

CPushButton::CPushButton(QWidget *parent)
    : QPushButton(parent)
{

}

void CPushButton::setButtonStyle(QString imgsrc, int CutSec)
{
    int img_w=QPixmap(imgsrc).width();
    int img_h=QPixmap(imgsrc).height();
    int PicWidth = img_w/CutSec;
    setFixedSize(PicWidth,img_h);
    setStyleSheet(QString("QPushButton{border-width: 41px; border-image: url(%1) 0 0 0 %2 repeat repeat;border-width: 0px; border-radius: 0px;}")
    .append("QPushButton::hover{border-image: url(%1) 0 0 0 %3 repeat repeat;}")
    .append("QPushButton::pressed{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
    .append("QPushButton::checked{border-image: url(%1) 0 0 0 %4 repeat repeat;}")
    .append("QPushButton::disabled{border-image: url(%1) 0 0 0 %5 repeat repeat;}")
    .arg(imgsrc).arg(0).arg(PicWidth*1).arg(PicWidth*2).arg(PicWidth*3));
}

