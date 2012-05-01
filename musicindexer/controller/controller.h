#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPair>

#include "logging/ILogger.h"
//#include "ui/mainwindow.h"
#include "ui/fullscreenview.h"
#include "actionimport.h"
#include "actionsearch.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    void init(QWidget *view);

Q_SIGNALS:

    void askForDirectory (QString*);
    void showSearchResult(const QStringList&);
    void showSimilarity  (const QStringList&);
    void showCollection  (const QStringList&);
    void showProgress    ();
    void updateProgress  (int);
    void hideProgress    ();


public Q_SLOTS:

    void slotImportStarted    ();
    void slotActionProgress   (int);
    void slotActionError      (const QString &);
    void slotActionEnded      (bool);
    void slotSearch           (const QString &query);
    void slotAddResources     ();
    void slotCollectionClicked();
    void slotSimilarityClicked();
    void slotSearchFinished   (const QStringList&);
    void slotShutDown         ();

private Q_SLOTS:

    void slotSongWithEmptyTerms(const QString& file, const QString& msg);


private:

    void connectStandarActionSignals(MBAction * action);
    void loadPendingFiles();

    QScopedPointer<ILogger>      _logger;
    //QScopedPointer<MainWindow>   _mainwindow;
    QScopedPointer<FullScreenView> _mainwindow;

    // Actions
    QScopedPointer<ActionImport>    _actionImport;
    QScopedPointer<ActionSearch>    _actionSearch;

    QList<QPair<QString, QString> > _pendingSongs;
    QFile                           _pendingSongsFile;
};


#endif // CONTROLLER_H
