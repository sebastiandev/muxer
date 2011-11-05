#include "actionsearch.h"
#include "logging/LoggerManager.h"
#include "musicmanager.h"
#include <QDebug>

ActionSearch::ActionSearch(const QString &query):_query(query)
{
}


bool ActionSearch::prepare()
{
}

void ActionSearch::runAction()
{
    LoggerManager::LogInfo("Searching for " + _query);
    QStringList results = MusicManager::manager().search(_query);

    //foreach(const QString &r, results)
    //    qDebug() << "Match: " << r;

    emit searchFinished(results);
}

bool ActionSearch::cleanup()
{
}

