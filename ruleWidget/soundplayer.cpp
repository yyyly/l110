#include "soundplayer.h"

SoundPlayer::SoundPlayer(QObject *parent)
    :QMediaPlayer(parent)
{
    connect(this,&SoundPlayer::stateChanged,this,&SoundPlayer::stateChange);
}

void SoundPlayer::playSound(const QString &adr, int num)
{
    if(num == 0)
        return;
    playNum = num;
    setMedia(QUrl::fromLocalFile(adr));
    play();
}

void SoundPlayer::stateChange()
{
    if(state() == State::StoppedState && --playNum > 0)
    {
        play();
    }
}

void SoundPlayer::stopPlay()
{
    playNum = 1;
    this->stop();
}
