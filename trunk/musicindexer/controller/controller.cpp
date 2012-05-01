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
      IConfigurator *configurator = (IConfigurator *) new IniConfigurator("resources/config.conf");
    #else
      _logger.reset((ILogger *)new FileLogger());
      IConfigurator *configurator = (IConfigurator *) new IniConfigurator("resources/config.conf");
    #endif

    ConfigurationManager::SetConfigurator(configurator);

    // Set up Logger
    LoggerManager::SetLogger(_logger.data());
    LoggerManager::SetLogLevel(LoggerManager::Debug);
}

void Controller::init()
{
    _mainwindow.reset(new MainWindow());

    QApplication::setApplicationName("Muxer");

    connect(_mainwindow.data(), SIGNAL(searchTrigger(const QString&)), this, SLOT(slotSearch(const QString&)));
    connect(_mainwindow.data(), SIGNAL(addResourcesTrigger())        , this, SLOT(slotAddResources()));
    connect(_mainwindow.data(), SIGNAL(getCollectionTrigger())       , this, SLOT(slotCollectionClicked()));
    connect(_mainwindow.data(), SIGNAL(showSimilarityTrigger())      , this, SLOT(slotSimilarityClicked()));

    connect(this              , SIGNAL(showSearchResult(const QStringList&)), _mainwindow.data(), SLOT(slotShowSongs(const QStringList&)));
    connect(this              , SIGNAL(showCollection  (const QStringList&)), _mainwindow.data(), SLOT(slotShowSongs(const QStringList&)));
    connect(this              , SIGNAL(showSimilarity  (const QStringList&)), _mainwindow.data(), SLOT(slotShowSimilarity(const QStringList&)));
    connect(this              , SIGNAL(showProgress())                      , _mainwindow.data(), SLOT(slotShowProgress()));
    connect(this              , SIGNAL(updateProgress(int))                 , _mainwindow.data(), SLOT(slotUpdateProgress(int)));
    connect(this              , SIGNAL(hideProgress())                      , _mainwindow.data(), SLOT(slotHideProgress()));
    connect(this              , SIGNAL(askForDirectory(QString*))           , _mainwindow.data(), SLOT(slotAskForDirectory(QString*)));

    _mainwindow->show();
}

void Controller::connectStandarActionSignals(MBAction * action)
{
    //connect(action, SIGNAL(actionStarted())             , this , SLOT(slotActionStarted()));
    connect(action, SIGNAL(actionProgress(int))         , this , SLOT(slotActionProgress(int)));
    connect(action, SIGNAL(actionError(const QString &)), this , SLOT(slotActionError(const QString &)));
    connect(action, SIGNAL(actionEnded(bool))           , this , SLOT(slotActionEnded(bool)));
}

void Controller::slotImportStarted()
{
    Q_EMIT showProgress();
}

void Controller::slotActionProgress(int progress)
{
    Q_EMIT updateProgress(progress);
}

void Controller::slotActionError(const QString &)
{
}

void Controller::slotActionEnded(bool)
{
    Q_EMIT hideProgress();
}


void Controller::slotSearch(const QString &query)
{
    if (query.isEmpty())
        return;

    _actionSearch.reset(new ActionSearch(query));

    connect(_actionSearch.data(), SIGNAL(searchFinished(QStringList)), this, SLOT(slotSearchFinished(const QStringList&)));
    connectStandarActionSignals(_actionSearch.data());

    _actionSearch->execute();
}

void Controller::slotSearchFinished(const QStringList &result)
{
    Q_EMIT showSearchResult(result);
}


void Controller::slotAddResources()
{
    QString directory;

    Q_EMIT askForDirectory(&directory);

    if (directory.isEmpty())
        return;

    _actionImport.reset(new ActionImport(QDir(directory)));

    connect(_actionImport.data(), SIGNAL(actionStarted()) , this , SLOT(slotImportStarted()));

    connectStandarActionSignals(_actionImport.data());

    _actionImport->execute();
}

void Controller::slotCollectionClicked()
{
    Q_EMIT showCollection(MusicManager::manager().getAllSongs());
}

void Controller::slotSimilarityClicked()
{
    Q_EMIT showSimilarity(MusicManager::manager().getAllSongs());
}
