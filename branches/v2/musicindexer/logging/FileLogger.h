#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include <iostream>

#include "ILogger.h"

class FileLogger : public ILogger
{
public:

    void LogDebug(QString message);
    void LogInfo(QString message);
    void LogWarn(QString message);
    void LogError(QString message);

private:

    void writeToFile(QString type, QString message);
};


#endif // QDEBUG_LOGGER_H
