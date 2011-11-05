#ifndef QDEBUG_LOGGER_H
#define QDEBUG_LOGGER_H

#include <iostream>
#include <QDebug>

#include "ILogger.h"

class QDebugLogger : public ILogger
{
public:

    void LogDebug(QString message);
    void LogInfo(QString message);
    void LogWarn(QString message);
    void LogError(QString message);
};



#endif // QDEBUG_LOGGER_H
