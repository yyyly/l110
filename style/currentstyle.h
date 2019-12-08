#ifndef CURRENTSTYLE_H
#define CURRENTSTYLE_H
#include<QProxyStyle>

class CurrentStyle : public QProxyStyle
{
public:
    CurrentStyle();
protected:
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
private:
    int mSize;
};

#endif // CURRENTSTYLE_H
