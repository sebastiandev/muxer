// QT Includes
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QUuid>

#include "QFileUtils.h"

bool QFileUtils::getTemporaryFilename(QString & filename)
{
    filename = QDir::tempPath() + "/" + QUuid::createUuid().toString().right(22);
    return true;
}

qint64 QFileUtils::getFileSize(const QStringList &files)
{
    qint64 totalSize = 0;

    foreach(const QString & file, files)
    {
        QFileInfo fileInfo(file);

        if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." )
            continue;

        if ( fileInfo.isDir() )
        {
            QDir dir = fileInfo.dir();
            dir.cd(fileInfo.fileName());
            QStringList subFiles = dir.entryList();
            QStringList subFilesWithPath;
            foreach (const QString & subFile, subFiles) //add the absolute path to each entry of the list
            {
                if (subFile == "." || subFile == ".." )
                    continue;

                subFilesWithPath.push_back(QString("%1/%2").arg(fileInfo.absoluteFilePath()).arg(subFile));
            }
            totalSize += getFileSize(subFilesWithPath);
        }
        else
        {
            totalSize += fileInfo.size();
        }
    }

    return totalSize;
}

qint64 QFileUtils::getFileSize(const QString &file)
{
    QFileInfo fileInfo(file);
    return fileInfo.size();
}

QStringList QFileUtils::getFilesInDirectory(const QString &path)
{
    QStringList files;

    QFileInfo fileInfo(path);
    if (fileInfo.isDir())
    {
        QDir dir = fileInfo.dir();
        dir.cd(fileInfo.fileName());

        QStringList subFiles = dir.entryList();
        foreach (const QString & subFile, subFiles) //add the absolute path to each entry of the list
        {
            if (subFile == "." || subFile == ".." )
                continue;

            files.append(getFilesInDirectory(QString("%1/%2").arg(fileInfo.absoluteFilePath()).arg(subFile)));
        }
    }
    else
        //files.append(QString("%1/%2").arg(fileInfo.absoluteFilePath()).arg(fileInfo.fileName()));
        files.append(path);

    return files;
}
