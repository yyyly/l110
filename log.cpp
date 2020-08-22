#include "log.h"
#include "log4qt/logger.h"
#include "log4qt/patternlayout.h"
#include "log4qt/dailyrollingfileappender.h"
#include "QCoreApplication"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::initLog()
{
    //配置日志系统
    //Log4Qt::PropertyConfigurator::configure(a.applicationDirPath()
     //   + "/log4.conf");
    //Log4Qt::DailyRollingFileAppender::setFile(a.applicationDirPath() + "/logs/log/log");

    Log4Qt::PatternLayout *p_layout = new Log4Qt::PatternLayout(this);
    p_layout->setConversionPattern("%d{yyyy-MM-dd HH:mm:ss} [%c] - %m%n");
    p_layout->activateOptions();
    const QString rFileName =QCoreApplication::applicationDirPath() + "/logs/log.log";
    const QString datePattern = "'.'yyyy-MM-dd";
    Log4Qt::DailyRollingFileAppender *appender = new Log4Qt::DailyRollingFileAppender(p_layout,rFileName,datePattern,this);
    appender->setAppendFile(true);
    appender->activateOptions();

    Log4Qt::Logger::rootLogger()->addAppender(appender);
}
