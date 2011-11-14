#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "database/songfinder.h"
#include "database/songindexer.h"
#include "entities/album.h"
#include "entities/artist.h"
#include "similaritymanager.h"

class MusicManager
{
public:

    ~MusicManager();

    static MusicManager& manager();

    void addSongsFromDirectory(QString dirPath);
    void addSongs(QStringList songsPath);
    void addSong(QString songPath);

    QStringList search(QString query);
    QStringList getAllSongs();
    QList<Album> getAlbumsByArtist(const QString &artist);
    Artist getArtist(const QString &artistName);



private:

    MusicManager(SongIndexer songIndexer, SongFinder songFinder);

    Xapian::WritableDatabase _db;

    SongIndexer       _indexer;
    SongFinder        _finder;
    //SimilarityManager _similarityManager;

};

#endif // MUSICMANAGER_H
