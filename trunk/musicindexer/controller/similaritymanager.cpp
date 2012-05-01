#include "similaritymanager.h"
#include <QMap>

#include "database/similaritydataservice.h"
#include "util/entitiesutil.h"

SimilarityManager::SimilarityManager()
{
}

SimilarityManager& SimilarityManager::manager()
{
    static SimilarityManager instance;
    return instance;
}

QList<Artist> SimilarityManager::getSimilarArtists(const Artist &artist)
{
    QList<Artist> similarArtists;

    Q_FOREACH(const Album &album, artist.getAlbums())
    {
        QList<Album> similarAlbums = getSimilarAlbums(album);
        Q_FOREACH(const Album &similarAlbum, similarAlbums)
        {
            //skip albums from the same artist
            if (similarAlbum.getArtist() == artist.getName())
                continue;

            Artist sArtist(similarAlbum.getArtist());//TODO: add every albums with every song in the artist variable!
            if (!similarArtists.contains(sArtist))
                similarArtists.append(sArtist);
        }
    }

    return similarArtists;
}

QList<Album>  SimilarityManager::getSimilarAlbums(const Album &album)
{
    AlbumDataObject albumData = SimilarityDataService::service().getAlbumData(album);

    QList<AlbumDataObject> result = searchSimilarAlbums(album, albumData);

    QList<Album> similarAlbums;
    Q_FOREACH(const AlbumDataObject &r, result)
    {
        Album a = EntitiesUtil::getAlbumDataFromDir(r.getPath());

        if (!similarAlbums.contains(a) && a!=album) //add only once
            similarAlbums.append(a);
    }

    return similarAlbums;
}

QList<AlbumDataObject> SimilarityManager::searchSimilarAlbums(const Album &album, AlbumDataObject &albumData)
{
    QPair<int, int> tempoRange;
    tempoRange.first  = albumData.getAverageTempo() - 15;
    tempoRange.second = albumData.getAverageTempo() + 15;

    QList<QPair<QString, int> > genresRanking = albumData.getGenreOcurrencies();
    QList<QPair<QString, int> > TypesRanking  = albumData.getTypeOcurrencies();

    QStringList genres, types;

    // choose only the first genre from the ranking and those whose frecuency is
    // greater than half of the first and greater thatn the diferences betwen the first and the next
    // ej:  8 - a
    //      6 - b  --> it will add genre b because its > 8/2 (first genre a) and > (8-3) (diference between genre a and c)
    //      3 - c
    //
    // ej:  8 - a
    //      4 - b  --> it will NOT add genre b because its > 8/2 (first genre a) BUT < (8-3) (diference between genre a and c)
    //      3 - c
    // this way we avoid adding genres that are between gaps and may not be really relevant,

    for (int i=0; i<genresRanking.size(); i++)
    {
        if (i == 0)
            genres.append(genresRanking.at(i).first);

        if (i>0 && genresRanking.at(i).second > (genresRanking.at(0).second / 2) && genresRanking.at(i).second > (genresRanking.at(0).second - genresRanking.at(i-1).second ))
            genres.append(genresRanking.at(i).first);
    }

    return SimilarityDataService::service().getAlbumsBy(album, tempoRange, genres, types);
}

void SimilarityManager::addAlbum(const QString &file, const Album &album)
{
    QList< QPair<QString, int> > genresRanking = calculateMostCommonAttribute(album, GENRE);
    QList< QPair<QString, int> > typesRanking  = calculateMostCommonAttribute(album, TYPE);

    SimilarityDataService::service().addAlbumData(file, album, genresRanking, typesRanking);
}

void SimilarityManager::addArtist(const Artist &artist)
{

}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int SimilarityManager::calculateAverageTempo(const Artist &artist)
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
class AuxItem
{
public:
    QString _key;
    int _ocurrences;

    AuxItem(){ _ocurrences = 0;}
    AuxItem(QString key, int ocurrences){ _key = key; _ocurrences = ocurrences;}
    ~AuxItem(){}

    inline bool operator< (const AuxItem &a1) const { return _ocurrences <  a1._ocurrences;}

};
////////////////////////////////////////////////////////////////////////////////////
QList<QString> SimilarityManager::calculateMostCommonGenres(const Artist &artist)
{
    const QList<Album> &albums = artist.getAlbums();
    Q_FOREACH(const Album &album, albums)
    {
    }

    return QList<QString>();
}

QList<QPair<QString, int> > SimilarityManager::calculateMostCommonAttribute(const Album &album, int ATTR)
{
    QMap<QString, AuxItem*> attrMap;

    const QList<Song> &songs = album.getSongs();
    Q_FOREACH(const Song &song, songs)
    {
        QList<QString> attributeList = (ATTR == GENRE ? song.getGenres() : song.getType().split(","));
        Q_FOREACH(QString attribute, attributeList)
        {
            if (!attribute.isEmpty())
            {
                attribute = attribute.trimmed();

                if (!attrMap.contains(attribute))
                    attrMap.insert(attribute, new AuxItem(attribute, 1));
                else
                    ((AuxItem*)attrMap.value(attribute))->_ocurrences++;
            }
        }
    }

    // build the descending ranking traversing the map
    QMapIterator<QString, AuxItem*> itMap(attrMap);
    QList<AuxItem> attributeRanking;
    while (itMap.hasNext())
    {
        itMap.next();
        attributeRanking.append(*itMap.value());
    }

    qSort(attributeRanking);//sort (by default is ascending)

    QList<QPair<QString, int> > finalRanking;
    Q_FOREACH(const AuxItem &item, attributeRanking)
        finalRanking.prepend(QPair<QString, int>(item._key, item._ocurrences));//add prepeding to build a decending list

    return finalRanking;
}
