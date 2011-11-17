#ifndef COMPACTPLAYLIST_H
#define COMPACTPLAYLIST_H

#include <QWidget>
#include <QModelIndex>
#include <QPair>

#include "entities/song.h"

namespace Ui {
    class CompactPlaylist;
}

class CompactPlaylist : public QWidget
{
    Q_OBJECT

public:
    explicit CompactPlaylist(QWidget *parent = 0);
    ~CompactPlaylist();

    void addSong   (const Song &song, const QString &file);
    int currentSong();
    void nextSong  ();

    bool contains(const Song &song);

    void clear     ();
    int  size      () { return _playlist.size();}


signals:

    void playSong(const Song&, const QString&);

private slots:

    void slotSongSelected(int row, int col);


private:
    Ui::CompactPlaylist *ui;

    QMap<QString, QPair<Song, QString> > _playlist;
    int _currentSong;
};

#endif // COMPACTPLAYLIST_H
