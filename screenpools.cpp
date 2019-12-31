#include "screenpools.h"
#include "screen.h"
const int addsize = 2;

ScreenPools::ScreenPools(QWidget *parent)
    :parentWidget(parent)
{
    Screen *s;
    screenVec.resize(2);
    int i = 0;
    foreach (s, screenVec) {
        s = new Screen(0,parentWidget);
        s->flag = 1;
        s->hide();
        screenVec[i++] = s;
        parent->installEventFilter(s);
    }

}
ScreenPools::~ScreenPools()
{
    Screen *s;
    foreach(s,screenVec)
    {
        if(s!= nullptr)
        {
            delete s;
        }
    }
}

Screen *ScreenPools::getSceen()
{
    Screen *s;
    foreach (s, screenVec) {
        if(s->getPlayState() == Screen::UNPLAY)
        {
            break;
        }
    }
    if(s->getPlayState() == Screen::PLAY)//说明已有的都处于播放状态，需要加大内存
    {
          int size = screenVec.size();
          screenVec.resize(size + addsize);
          for(int i = 0;i < addsize;i++)
          {
              s = new Screen(0,parentWidget);
              s->hide();
              s->flag = 1;
              screenVec[size + i] = s;
          }
    }
    else
    {
        return  s;
    }
}
