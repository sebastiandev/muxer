#ifndef GFILEUTILS_H
#define GFILEUTILS_H

#include <QStringList>
#include <QString>

class QFileUtils
{
public:
    static bool        getTemporaryFilename(QString & filename);
    static qint64      getFileSize(const QStringList &files);
    static qint64      getFileSize(const QString &file);
    static QStringList getFilesInDirectory(const QString &path);
    static int         howManyFiles(const QString &path, const QStringList &filter=QStringList(), bool recursive=true);

private:

    static void        countFilesInDir(const QString &path, const QStringList &filter, int &count);

};

#endif // GFILEUTILS_H
