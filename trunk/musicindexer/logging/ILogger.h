#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>

class ILogger
{
public:
    virtual void LogDebug(QString message) = 0;
    virtual void LogInfo (QString message) = 0;
    virtual void LogWarn (QString message) = 0;
    virtual void LogError(QString message) = 0;
};

#endif // ILOGGER_H
