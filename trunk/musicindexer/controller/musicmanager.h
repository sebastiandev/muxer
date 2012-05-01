#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "database/songfinder.h"
#include "database/songindexer.h"
#include "entities/album.h"
#include "entities/artist.h"
#include <QObject>

class MusicManager : public QObject
{
    Q_OBJECT

public:

    ~MusicManager();

    static MusicManager& manager();

    void addSongsFromDirectory(QString dirPath);
    void addSongs             (QStringList songsPath);
    void addSong              (QString songPath);

    QStringList  search           (QString query);
    QStringList  getAllSongs      ();
    QList<Album> getAlbumsByArtist(const QString &artist);
    Artist       getArtist        (const QString &artistName);


Q_SIGNALS:

    void indexing(const QString&, int, int);

public Q_SLOTS:


private:

    MusicManager(){}
    MusicManager(SongIndexer songIndexer, SongFinder songFinder);

    QStringList loadStopWords();

    static MusicManager      *instance;
    Xapian::WritableDatabase _db;
    SongIndexer              _indexer;
    SongFinder               _finder;

};

#endif // MUSICMANAGER_H
