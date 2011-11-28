#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "logging/ILogger.h"
#include "ui/mainwindow.h"
#include "actionimport.h"
#include "actionsearch.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    void init();

signals:

    void askForDirectory (QString*);
    void showSearchResult(const QStringList&);
    void showSimilarity  (const QStringList&);
    void showCollection  (const QStringList&);
    void showProgress    ();
    void updateProgress  (int);
    void hideProgress    ();


public slots:

    void slotImportStarted();
    void slotActionProgress(int);
    void slotActionError(const QString &);
    void slotActionEnded(bool);

    void slotSearch(const QString &query);
    void slotAddResources();
    void slotCollectionClicked();
    void slotSimilarityClicked();
    void slotSearchFinished(const QStringList&);

private:

    void connectStandarActionSignals(MBAction * action);

    QScopedPointer<ILogger>       _logger;
    QScopedPointer<MainWindow>    _mainwindow;

    // Actions
    QScopedPointer<ActionImport> _actionImport;
    QScopedPointer<ActionSearch> _actionSearch;

};

#endif // CONTROLLER_H
