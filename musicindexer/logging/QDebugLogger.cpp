#include "QDebugLogger.h"


void QDebugLogger::LogInfo(QString message)
{
    qDebug() << message;
}

void QDebugLogger::LogError(QString message)
{
    qCritical() << message;
}

void QDebugLogger::LogDebug(QString message)
{
    qDebug() << message;
}

void QDebugLogger::LogWarn(QString message)
{
    qWarning() << message;
}

