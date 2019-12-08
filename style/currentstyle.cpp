#include "currentstyle.h"

CurrentStyle::CurrentStyle()
{
    mSize = 32;
}

int CurrentStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    if(metric == QStyle::PM_SmallIconSize)
        return mSize;
    return QProxyStyle::pixelMetric(metric,option,widget);
}
