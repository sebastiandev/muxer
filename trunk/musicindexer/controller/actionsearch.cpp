#include "actionsearch.h"
#include "logging/LoggerManager.h"
#include "musicmanager.h"
#include <QDebug>

ActionSearch::ActionSearch(const QString &query):_query(query)
{
}


bool ActionSearch::prepare()
{
    return true;
}

void ActionSearch::runAction()
{
    LoggerManager::LogInfo("Searching for " + _query);
    QStringList results = MusicManager::manager().search(_query);

    //Q_FOREACH(const QString &r, results)
    //    qDebug() << "Match: " << r;

    Q_EMIT searchFinished(results);
}

bool ActionSearch::cleanup()
{
    return true;
}

