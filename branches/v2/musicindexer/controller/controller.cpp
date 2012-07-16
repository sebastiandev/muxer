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

void Controller::init(QWidget* view)
{
    //_mainwindow.reset(new MainWindow());
    _mainwindow.reset((FullScreenView*)view);

    loadPendingSongsToImport();

    connect(_mainwindow.data(), SIGNAL(searchTrigger(const QString&)), this, SLOT(slotSearch(const QString&)));
    //connect(_mainwindow.data(), SIGNAL(addResourcesTrigger())        , this, SLOT(slotAddResources()));
    connect(_mainwindow.data(), SIGNAL(importFolderRequest(QString&))       , this, SLOT(slotAddResources(QString&)));
    //connect(_mainwindow.data(), SIGNAL(getCollectionTrigger())       , this, SLOT(slotCollectionClicked()));
    connect(_mainwindow.data(), SIGNAL(showCollectionRequest())             , this, SLOT(slotCollectionClicked()));
    connect(_mainwindow.data(), SIGNAL(showSimilarityTrigger())      , this, SLOT(slotSimilarityClicked()));
    connect(_mainwindow.data(), SIGNAL(quit())                       , this, SLOT(slotShutDown()));

    //connect(this              , SIGNAL(showSearchResult(const QStringList&)), _mainwindow.data(), SLOT(slotShowSongs(const QStringList&)));
    //connect(this              , SIGNAL(showCollection  (const QStringList&)), _mainwindow.data(), SLOT(slotShowSongs(const QStringList&)));
    connect(this              , SIGNAL(showSearchResult(const QStringList&)), _mainwindow.data(), SLOT(slotShowMusic(const QStringList&)));
    connect(this              , SIGNAL(showCollection  (const QStringList&)), _mainwindow.data(), SLOT(slotShowMusic(const QStringList&)));

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


void Controller::slotAddResources(QString &directory)
{
    //QString directory;
    //Q_EMIT askForDirectory(&directory);

    if (directory.isEmpty())
        return;

    _actionImport.reset(new ActionImport(QDir(directory)));

    connect(_actionImport.data(), SIGNAL(actionStarted()) , this , SLOT(slotImportStarted()));
    connect(_actionImport.data(), SIGNAL(songWithEmptyTerms(QString,QString)) , this , SLOT(slotTrackSongWithEmptyTerms(QString, QString)));

    connectStandarActionSignals(_actionImport.data());

    _actionImport->execute();
}

void Controller::slotTrackSongWithEmptyTerms(const QString &file, const QString &msg)
{
    _pendingSongs << QPair<QString, QString>(file, msg);
}

void Controller::slotCollectionClicked()
{
    Q_EMIT showCollection(MusicManager::manager().getAllSongs());
}

void Controller::slotSimilarityClicked()
{
    Q_EMIT showSimilarity(MusicManager::manager().getAllSongs());
}

void Controller::loadPendingSongsToImport()
{
    _pendingSongsFile.setFileName(ConfigurationManager::GetString("pendingFiles"));
    if (_pendingSongsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream in(&_pendingSongsFile);
        while (!in.atEnd())
        {
            QStringList item = in.readLine().split(":");
            _pendingSongs << QPair<QString, QString>(item.first(), item.at(1));
        }
    }
}

void Controller::slotShutDown()
{
    // save pending files, if any, to be recovered next time
    if (!_pendingSongs.isEmpty())
    {
        if (_pendingSongsFile.reset())
        {
            QTextStream out(&_pendingSongsFile);
            for (int i=0; i<_pendingSongs.size(); i++)
            {
                out << _pendingSongs.at(i).first  <<  ": ";
                out << _pendingSongs.at(i).second << "\n";
            }
        }
    }
}
