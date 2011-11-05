#ifndef SIMILARITYDATASERVICE_H
#define SIMILARITYDATASERVICE_H

#include <QString>
#include <QPair>
#include <QList>
#include <QSqlDatabase>

#include "entities/album.h"

class QSqlDatabase;
class AlbumDataObject;

class SimilarityDataService
{
public:

    ~SimilarityDataService(){}

    static SimilarityDataService& service();

    void            addAlbumData(const QString &file, const Album &album, const QList< QPair<QString, int> > &genres, const QList< QPair<QString, int> > &types);

    AlbumDataObject getAlbumData(const Album &album);
    AlbumDataObject getAlbumData(const QString &id);

    QList<AlbumDataObject> getAlbumsBy(const Album& album, QPair<int, int> tempoRange, QStringList genres, QStringList types);

private:

    SimilarityDataService();

    QSqlDatabase _db;

};

class AlbumDataObject
{
public:

    AlbumDataObject(){}
    AlbumDataObject(const QString &artist, const QString &title){ _artist = artist; _title = title;}
    AlbumDataObject(const QString &artist, const QString &title, const int &avgTempo){ _artist = artist; _title = title; _averageTempo = avgTempo;}

    void setArtist(const QString &artist ) { _artist = artist;}
    void setTitle (const QString &title  ) { _title = title;}
    void setAverageTempo(const int &tempo) { _averageTempo = tempo;}
    void setPath(const QString &path){ _path = path;}

    void addGenreOcurrency(QPair<QString, int> pair){ _genreOcurrencies.append(pair);}
    void addTypeOcurrency (QPair<QString, int> pair){ _typeOcurrencies.append(pair);}

    QString getArtist() const { return _artist;}
    QString getTitle () const { return _title;}
    QString getPath  () const { return _path;}

    int     getAverageTempo() const { return _averageTempo;}

    QList<QPair<QString, int> > getGenreOcurrencies(){ return _genreOcurrencies;}
    QList<QPair<QString, int> > getTypeOcurrencies (){ return _typeOcurrencies;}

private:

    QString _artist, _title, _path;
    int     _averageTempo;
    QList<QPair<QString, int> > _genreOcurrencies;
    QList<QPair<QString, int> > _typeOcurrencies;

};

#endif // SIMILARITYDATASERVICE_H
