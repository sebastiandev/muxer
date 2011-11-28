#include "actionimport.h"
#include "logging/LoggerManager.h"
#include "musicmanager.h"
#include <QDebug>

ActionImport::ActionImport(QStringList list):_filesToImport(list)
{
    connect(&MusicManager::manager(), SIGNAL(indexing(QString,int ,int)), this, SLOT(calculateProgress(QString, int, int)), Qt::DirectConnection);
}
ActionImport::ActionImport(QDir dir):_dirToImport(dir)
{
    connect(&MusicManager::manager(), SIGNAL(indexing(QString,int ,int)), this, SLOT(calculateProgress(QString, int, int)), Qt::DirectConnection);
}


bool ActionImport::prepare()
{
}

void ActionImport::runAction()
{
    LoggerManager::LogInfo("Importing files");

    if (_dirToImport.exists())
    {
        emit actionStarted();
        MusicManager::manager().addSongsFromDirectory(_dirToImport.absolutePath());
        emit actionEnded(true);
    }
}

bool ActionImport::cleanup()
{
}

void ActionImport::calculateProgress(QString file, int count, int total)
{
    int percentage = (count * 100) / total;
    //qDebug() << "indexing file: " << file << ". " << count << "/" << total << " --> " << percentage << "%";

    emit actionProgress(percentage);
}
