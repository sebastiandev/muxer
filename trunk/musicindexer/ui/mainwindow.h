#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QPoint>
#include <QEvent>
#include "musicviewer.h"
#include "entities/song.h"
#include "compactplayer.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void searchTrigger(const QString&);
    void addResourcesTrigger();
    void getCollectionTrigger();
    void showSimilarityTrigger();

public slots:
    void slotAskForDirectory(QString *dir);
    void slotShowSongs(const QStringList &songlist);
    void slotShowSimilarity(const QStringList &songlist);
    void slotPlaySongRequested(const Song&, const QString&);

private slots:
    void highlightInput();
    void searchClicked();
    void slotClose();

private:

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject *obj, QEvent *e);

    Ui::MainWindow ui;
    QPoint         m_dragPosition;
    MusicViewer    *_musicViewer;
    CompactPlayer  *_player;
};

#endif // MAINWINDOW_H
