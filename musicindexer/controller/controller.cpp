#include "controller.h"
#include "util/QFileUtils.h"
#include "logging/FileLogger.h"
#include "logging/LoggerManager.h"
#include "logging/QDebugLogger.h"
#include "configuration/IniConfigurator.h"
#include "configuration/ConfigurationManager.h"
#include "musicmanager.h"

Controller::Controller(QObject *parent) :
    QObject(parent)
{

    #ifdef DEBUG
      _logger.reset((ILogger *)new QDebugLogger());
    #else
      _logger.reset((ILogger *)new FileLogger());
    #endif

    IConfigurator *configurator = (IConfigurator *) new IniConfigurator("resources/config.conf");
    ConfigurationManager::SetConfigurator(configurator);

    // Set up Logger
    LoggerManager::SetLogger(_logger.data());
    LoggerManager::SetLogLevel(LoggerManager::Debug);
}

void Controller::init()
{
    _mainwindow.reset(new MainWindow());
    _mainwindow->show();

    connect(_mainwindow.data(), SIGNAL(searchTrigger(const QString&))        , this, SLOT(slotSearch(const QString&)));
    connect(_mainwindow.data(), SIGNAL(addResourcesTrigger())  , this, SLOT(slotAddResources()));
    connect(_mainwindow.data(), SIGNAL(getCollectionTrigger()) , this, SLOT(slotCollectionClicked()));
    connect(_mainwindow.data(), SIGNAL(showSimilarityTrigger()), this, SLOT(slotSimilarityClicked()));

    connect(this              , SIGNAL(showSearchResult(const QStringList&)), _mainwindow.data(), SLOT(slotShowSongs(const QStringList&)));
    connect(this              , SIGNAL(showCollection  (const QStringList&)), _mainwindow.data(), SLOT(slotShowSongs(const QStringList&)));
    connect(this              , SIGNAL(showSimilarity  (const QStringList&)), _mainwindow.data(), SLOT(slotShowSimilarity(const QStringList&)));

    connect(this, SIGNAL(askForDirectory(QString*)), _mainwindow.data(), SLOT(slotAskForDirectory(QString*)));
}

void Controller::connectStandarActionSignals(MBAction * action)
{
    connect(action, SIGNAL(actionStarted())             , this , SLOT(slotActionStarted()));
    connect(action, SIGNAL(actionProgress(int))         , this , SLOT(slotActionProgress(int)));
    connect(action, SIGNAL(actionError(const QString &)), this , SLOT(slotActionError(const QString &)));
    connect(action, SIGNAL(actionEnded(bool))           , this , SLOT(slotActionEnded(bool)));
}

void Controller::slotActionStarted()
{

}

void Controller::slotActionProgress(int)
{
}

void Controller::slotActionError(const QString &)
{
}

void Controller::slotActionEnded(bool)
{
}


void Controller::slotSearch(const QString &query)
{
    _actionSearch.reset(new ActionSearch(query));

    connect(_actionSearch.data(), SIGNAL(searchFinished(QStringList)), this, SLOT(slotSearchFinished(const QStringList&)));
    connectStandarActionSignals(_actionSearch.data());

    _actionSearch->execute();

}

void Controller::slotSearchFinished(const QStringList &result)
{
    emit showSearchResult(result);
}


void Controller::slotAddResources()
{
    QString directory;

    emit askForDirectory(&directory);

    if (directory.isEmpty())
        return;

    _actionImport.reset(new ActionImport(QDir(directory)));

    connectStandarActionSignals(_actionImport.data());

    _actionImport->execute();
}

void Controller::slotCollectionClicked()
{
    emit showCollection(MusicManager::manager().getAllSongs());
}

void Controller::slotSimilarityClicked()
{
    emit showSimilarity(MusicManager::manager().getAllSongs());
}
