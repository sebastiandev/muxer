#include "FileLogger.h"
#include <iostream>
#include <fstream>
#include <QDateTime>


void FileLogger::LogInfo(QString message)
{
   writeToFile(QString("Info"), message);
}

void FileLogger::LogError(QString message)
{
   writeToFile(QString("Error"), message);
}

void FileLogger::LogDebug(QString message)
{
   writeToFile(QString("Debug"), message);
}

void FileLogger::LogWarn(QString message)
{
   writeToFile(QString("Warning"), message);
}

void FileLogger::writeToFile(QString type, QString message)
{
   std::ofstream myfile;
   myfile.open ("musicbrowser.log", std::ios::out | std::ios::app);

   myfile << type.leftJustified(8,QChar(' ')).toStdString();
   myfile << " - ";
   myfile << QDateTime::currentDateTime().toString().toStdString() ;
   myfile << " - ";
   myfile << message.toStdString() << std::endl;
   myfile.close();      
}

