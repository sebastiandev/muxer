#include "actionimport.h"
#include "logging/LoggerManager.h"
#include "musicmanager.h"
#include <QDebug>

ActionImport::ActionImport(QStringList list):_filesToImport(list)
{
    connect(&MusicManager::manager(), SIGNAL(indexing(QString,int ,int)), this, SLOT(slotCalculateProgress(QString, int, int)), Qt::DirectConnection);
    connect(&MusicManager::manager(), SIGNAL(songWithEmptyTerms(QString,QString)), this, SIGNAL(songWithEmptyTerms(QString,QString)), Qt::DirectConnection);
}
ActionImport::ActionImport(QDir dir):_dirToImport(dir)
{
    connect(&MusicManager::manager(), SIGNAL(indexing(QString,int ,int)), this, SLOT(calculateProgress(QString, int, int)), Qt::DirectConnection);
    connect(&MusicManager::manager(), SIGNAL(songWithEmptyTerms(QString,QString)), this, SIGNAL(songWithEmptyTerms(QString,QString)), Qt::DirectConnection);
}


bool ActionImport::prepare()
{
    return true;
}

void ActionImport::runAction()
{
    LoggerManager::LogInfo("Importing files");

    if (_dirToImport.exists())
    {
        Q_EMIT actionStarted();
        MusicManager::manager().addSongsFromDirectory(_dirToImport.absolutePath());
        Q_EMIT actionEnded(true);
    }
}

bool ActionImport::cleanup()
{
    return true;
}

void ActionImport::slotCalculateProgress(QString file, int count, int total)
{
    int percentage = (count * 100) / total;
    //qDebug() << "indexing file: " << file << ". " << count << "/" << total << " --> " << percentage << "%";

    Q_EMIT actionProgress(percentage);
}

