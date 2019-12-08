#ifndef LOGEVENT_H
#define LOGEVENT_H

#include <QObject>
#include <QEvent>

class LogEvent : public QEvent
{
public:
    LogEvent();
    static QEvent::Type getEventType();
private:
    static QEvent::Type type;
};

#endif // LOGEVENT_H
