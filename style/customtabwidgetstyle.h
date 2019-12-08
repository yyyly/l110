#ifndef CUSTOMTABWIDGETSTYLE_H
#define CUSTOMTABWIDGETSTYLE_H
#include<QProxyStyle>


class CustomTabWidgetStyle : public QProxyStyle
{
public:
    CustomTabWidgetStyle();
    QSize sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;


};

#endif // CUSTOMTABWIDGETSTYLE_H
