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

    foreach (const Album &album, artist.getAlbums())
    {
        QList<Album> similarAlbums = getSimilarAlbums(album);
        foreach (const Album &similarAlbum, similarAlbums)
        {
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
    foreach (const AlbumDataObject &r, result)
    {
        //Album a(r.getTitle(), r.getArtist());//TODO: add every song in the album
        Album a = EntitiesUtil::getAlbumDataFromDir(r.getPath());

        if (!similarAlbums.contains(a)) //add only once
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

    // choose only the first genre from the ranking and those whose frecuency is greater than half of the first and half of the previous
    for (int i=0; i<genresRanking.size(); i++)
    {
        if (i == 0)
            genres.append(genresRanking.at(i).first);

        if (i>0 && genresRanking.at(i).second > (genresRanking.at(0).second / 2) && genresRanking.at(i).second > (genresRanking.at(i-1).second / 2))
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
    foreach(const Album &album, albums)
    {
    }

    return QList<QString>();
}

QList<QPair<QString, int> > SimilarityManager::calculateMostCommonAttribute(const Album &album, int ATTR)
{
    QMap<QString, AuxItem*> attrMap;

    const QList<Song> &songs = album.getSongs();
    foreach (const Song &song, songs)
    {
        QList<QString> attributeList = (ATTR == GENRE ? song.getGenres() : song.getType().split(","));
        foreach(QString attribute, attributeList)
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
    foreach(const AuxItem &item, attributeRanking)
        finalRanking.prepend(QPair<QString, int>(item._key, item._ocurrences));//add prepeding to build a decending list

    return finalRanking;
}
