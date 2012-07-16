#include "LoggerManager.h"
#include "ILogger.h"

ILogger* LoggerManager::_logger = 0;
LoggerManager::LogLevel LoggerManager::_logLevel = Error;

void LoggerManager::SetLogger(ILogger * logger)
{
  _logger = logger;
}

void LoggerManager::SetLogLevel(LogLevel level)
{
  _logLevel = level;
}

void LoggerManager::LogDebug(QString message)
{
    if(_logLevel >= Debug && _logger)
        _logger->LogDebug(message);
}

void LoggerManager::LogInfo(QString message)
{
    if(_logLevel >= Info &&_logger)
        _logger->LogInfo(message);
}

void LoggerManager::LogWarn(QString message)
{
    if(_logLevel >= Warning &&_logger)
        _logger->LogWarn(message);
}

void LoggerManager::LogError(QString message)
{
    if(_logLevel >= Error &&_logger)
        _logger->LogError(message);
}
