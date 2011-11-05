#include "actionimport.h"
#include "logging/LoggerManager.h"
#include "musicmanager.h"

ActionImport::ActionImport(QStringList list):_filesToImport(list)
{
}
ActionImport::ActionImport(QDir dir):_dirToImport(dir)
{
}


bool ActionImport::prepare()
{
}

void ActionImport::runAction()
{
    LoggerManager::LogInfo("Importing files");

    if (_dirToImport.exists())
    {
        MusicManager::manager().addSongsFromDirectory(_dirToImport.absolutePath());
    }
}

bool ActionImport::cleanup()
{
}

