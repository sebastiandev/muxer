#ifndef ENTITIESUTIL_H
#define ENTITIESUTIL_H

#include <QStringList>
#include <QList>

#include "entities/artist.h"
#include "entities/album.h"
#include "entities/song.h"

class EntitiesUtil
{
public:

    static Album         getAlbumDataFromDir (const QString &dir);
    static QList<Artist> getArtistsFromSongs (const QStringList &songs);
    static QList<Album>  getAlbumsFromSongs  (const QStringList &songs);
    static QString       getAlbumPathFromFile(const QString &filePath);


private:
    EntitiesUtil();

    static Album* getAlbumFromFile(const QString &file);

};

#endif // ENTITIESUTIL_H
