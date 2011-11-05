#ifndef SIMILARITYMANAGER_H
#define SIMILARITYMANAGER_H

#include "entities/artist.h"
#include "entities/album.h"
#include <QPair>

class AlbumDataObject;//forward declaration

class SimilarityManager
{
public:

    static SimilarityManager& manager();

    SimilarityManager(QList<Album> database){ _database = database;}

    QList<Artist> getSimilarArtists(const Artist &artist);
    QList<Album>  getSimilarAlbums(const Album  &album);

    void addAlbum (const QString &file, const Album  &album);
    void addArtist(const Artist &artist);

private:

    SimilarityManager();

    enum attributes{ GENRE, TYPE};

    int calculateAverageTempo(const Artist &artist);
    int calculateAverageTempo(const Album &album);
    QList<QString> calculateMostCommonGenres(const Artist &artist);
    QList<QPair<QString, int> > calculateMostCommonAttribute(const Album &album, int ATTR);
    QList<AlbumDataObject> searchSimilarAlbums(const Album &album, AlbumDataObject &albumData);


    QList<Album> _database;
};

#endif // SIMILARITYMANAGER_H
