#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QObject>
#include <QMediaPlayer>

class SoundPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    SoundPlayer(QObject *parent = nullptr);
    void playSound(const QString &adr,int num);
   public slots:
     void stateChange();
     void stopPlay();
private:
    int playNum;
};

#endif // SOUNDPLAYER_H
