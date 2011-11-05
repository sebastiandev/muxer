
#ifndef LOGGER_MANAGER_H
#define LOGGER_MANAGER_H

#include<QString>

 class ILogger;
 class LoggerManager
 {
 public:

     typedef enum
     {
         NoLogging,
         Error,
         Warning,
         Info,
         Debug

     } LogLevel;

     static void SetLogger(ILogger * logger);
     static void SetLogLevel(LogLevel level);
     static void LogInfo (QString message);
     static void LogError(QString message);
     static void LogDebug(QString message);
     static void LogWarn (QString message);

 private:
     static ILogger* _logger;
     static LogLevel _logLevel;
 };


#endif // LOGGER_MANAGER_H
