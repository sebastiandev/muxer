#include "filecrawler.h"
#include <QDir>

FileCrawler::FileCrawler(QObject *parent) :
    QObject(parent)
{
}

void FileCrawler::crawlDirectory(const QString &path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.isDir())
    {
        QDir dir = fileInfo.dir();
        dir.cd(fileInfo.fileName());

        QFileInfoList subFiles = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        foreach (const QFileInfo & subFile, subFiles)
        {
            crawlDirectory(subFile.absoluteFilePath());
        }
    }
    else
        emit foundFile(path);
}
