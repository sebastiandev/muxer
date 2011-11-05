#include "similaritydataservice.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlResult>

#include "configuration/ConfigurationManager.h"
#include "logging/LoggerManager.h"

SimilarityDataService::SimilarityDataService()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(ConfigurationManager::GetString("similaritydb"));

    if (!_db.open())
        qDebug() << _db.lastError().text();

    QSqlQuery query;

    QString str = "CREATE TABLE albums(artist        TEXT NOT NULL,"
                  "title         TEXT NOT NULL,"
                  "average_tempo INT,"
                  "path  TEXT NOT NULL,"
                  "PRIMARY KEY(artist, title));";

    if (!query.exec(str))
        qDebug() << query.lastError().text();


    str = "CREATE TABLE genres(artist    TEXT NOT NULL,"
                              "title     TEXT NOT NULL,"
                              "genre      TEXT NOT NULL,"
                              "ocurrency INT,"
                              "FOREIGN KEY(artist, title) REFERENCES albums(artist, title));";

    if (!query.exec(str))
        qDebug() << query.lastError().text();


    str = "CREATE TABLE types(artist    TEXT NOT NULL,"
                             "title     TEXT NOT NULL,"
                             "type      TEXT NOT NULL,"
                             "ocurrency INT,"
                             "FOREIGN KEY(artist, title) REFERENCES albums(artist, title));";

    if (!query.exec(str))
        qDebug() << query.lastError().text();

}

SimilarityDataService& SimilarityDataService::service()
{
    static SimilarityDataService instance;
    return instance;
}

void SimilarityDataService::addAlbumData(const QString &file, const Album &album, const QList< QPair<QString, int> > &genres, const QList< QPair<QString, int> > &types)
{
    QSqlQuery query;
    if (!query.exec("INSERT INTO albums (artist, title, average_tempo, path) VALUES ('" + album.getArtist().trimmed()+ "','" +
                                                                                    album.getTitle().trimmed()  + "'," +
                                                                                    QString::number(album.getAverageTempo()) +  "," +
                                                                                    "'" + file + "');"))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();

    for (int i=0; i<genres.size(); i++)
    {
        QPair<QString, int> pair = genres.at(i);
        if (!query.exec("INSERT INTO genres (artist, title, genre, ocurrency) VALUES ('" + album.getArtist().trimmed() + "','" +
                                                                                        album.getTitle().trimmed()  + "','" +
                                                                                        pair.first.trimmed() + "'," +
                                                                                        QString::number(pair.second) + ");"))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();
    }

    for (int i=0; i<types.size(); i++)
    {
        QPair<QString, int> pair = types.at(i);
        if (!query.exec("INSERT INTO types (artist, title, type, ocurrency) VALUES ('" + album.getArtist().trimmed() + "','" +
                                                                                        album.getTitle().trimmed()  + "','" +
                                                                                        pair.first.trimmed() + "'," +
                                                                                        QString::number(pair.second) + ");"))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();
    }

    LoggerManager::LogDebug("[Similarity Data Service] --------------------------------------------------");
    LoggerManager::LogDebug("[Similarity Data Service] Saving similarity data:");
    LoggerManager::LogDebug("[Similarity Data Service] Artist: " + album.getArtist());
    LoggerManager::LogDebug("[Similarity Data Service] Album: " + album.getTitle());
    LoggerManager::LogDebug("[Similarity Data Service] Average tempo: " + album.getAverageTempo());

    for (int i=0; i<genres.size();i++)
        LoggerManager::LogDebug("[Similarity Data Service] Genre: " + genres.at(i).first + ", Ranking: " + QString::number(genres.at(i).second) + ", ");

    for (int i=0; i<types.size();i++)
        LoggerManager::LogDebug("[Similarity Data Service] Type: " + types.at(i).first + ", Ranking: " + QString::number(types.at(i).second) + ", ");

    LoggerManager::LogDebug("[Similarity Data Service] --------------------------------------------------");
}

AlbumDataObject SimilarityDataService::getAlbumData(const Album &album)
{
    AlbumDataObject data;

    QSqlQuery query;
    if (!query.exec("SELECT artist, title, average_tempo, path FROM albums WHERE artist = '" + album.getArtist() +"' AND title = '"+ album.getTitle() +"';"))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();

    while (query.next())
    {
        data.setArtist(query.value(0).toString());
        data.setTitle(query.value(1).toString());
        data.setAverageTempo(query.value(2).toInt());
        data.setPath(query.value(3).toString());
    }

    if (!query.exec("SELECT artist, title, genre, ocurrency FROM genres WHERE artist = '" + album.getArtist() +"' AND title = '"+ album.getTitle() +"' ORDER BY ocurrency DESC;"))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();

    while (query.next())
    {
        data.addGenreOcurrency(QPair<QString, int> (query.value(2).toString(), query.value(3).toInt()));
    }


    if (!query.exec("SELECT artist, title, type, ocurrency FROM types WHERE artist = '" + album.getArtist() +"' AND title = '"+ album.getTitle() +"' ORDER BY ocurrency DESC;"))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();

    while (query.next())
    {
        data.addTypeOcurrency(QPair<QString, int> (query.value(2).toString(), query.value(3).toInt()));
    }

    return data;
}


AlbumDataObject SimilarityDataService::getAlbumData(const QString &id)
{
    return AlbumDataObject();
}

QList<AlbumDataObject> SimilarityDataService::getAlbumsBy(const Album& album, QPair<int, int> tempoRange, QStringList genres, QStringList types)
{
    QSqlQuery query;
    /************************
     * arma una query que filtra los albumes que estan entre el rango de tempos promedio y ademas
     * tienen los generos especificados y su frecuencia es mayor a la frecuencia promedio
     * menos la mitad de la distancia entre la maxima y minima, de los generos de ese album
     *
     * SELECT DISTINCT ... FROM albums INNER JOIN genres ON artist, title
     * WHERE <en rango de tempo> AND <generos especificados> AND <frecuencia del genero es mayor a (primedio menos la mitad de la distancia de max y min)>
     *
     ************************/
    QString select = "SELECT DISTINCT albums.artist, albums.title, albums.average_tempo, albums.path, genres.genre, genres.ocurrency FROM albums INNER JOIN genres ON albums.artist=genres.artist AND albums.title=genres.title ";
    QString where  = "WHERE albums.average_tempo <= " + QString::number(tempoRange.second) + " AND albums.average_tempo >= " + QString::number(tempoRange.first) + " ";

    if (genres.size() > 0)
    {
        where += "AND (";

        for(int i=0; i<genres.size(); i++)
        {
            QString genre = genres.at(i);

            if (i > 0)
                where += " OR ";

            where += "genres.genre = '" + genre + "'";
        }

        where += ")";
    }

    if (types.size() > 0)
    {
        where += "AND (";

        for(int i=0; i<genres.size(); i++)
        {
            QString type = genres.at(i);

            if (i > 0)
                where += " OR ";

            where += "types.type = '" + type + "'";
        }

        where += ")";
    }

    // we only want those genres that are more frecuent than the average, minus the diference bewteen the max and min, divided by 2
    where += " AND genres.ocurrency>(SELECT DISTINCT AVG(genres.ocurrency)-MIN(genres.ocurrency) FROM genres)";
    where += ";";

    if (!query.exec(select + where))
        qDebug() << query.lastError().text() << ". Query: " << query.lastQuery();

    QList<AlbumDataObject> list;
    while (query.next())
    {
        AlbumDataObject album(query.value(0).toString(), query.value(1).toString(), query.value(2).toInt());
        album.setPath(query.value(3).toString());
        album.addGenreOcurrency(QPair<QString, int>(query.value(3).toString(), query.value(4).toInt()));
        list.append(album);
    }

    return list;
}
