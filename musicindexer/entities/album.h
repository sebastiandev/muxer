#ifndef ALBUM_H
#define ALBUM_H

#include "song.h"
#include <QImage>
#include <QMetaType>


class Album
{
public:

    virtual ~Album(){}
    Album(){}
    Album(const QString &title){ _title = title;}
    Album(const QString &title, const QString &artist){ _title = title; _artist = artist;}
    Album(const QString &title, const QString &artist, const QString year){ _title = title;  _artist = artist;_year = year;}
    Album(const QString &title, const QString &artist, const QString year, const QList<Song> songs){ _title = title;  _artist = artist;_year = year; _songs = songs;}
    //Album(const QImage &cover, const QString &title, const QString year, const QList<Song> songs){ _cover = cover;_title = title; _year = year; _songs = songs;}


    QString getTitle () const { return _title;}
    QString getYear  () const { return _year;}
    QString getArtist() const { return _artist;}
    QImage  getCover () const { return _cover;}

    int getAverageTempo() const
    {
        int tempo=0;
        foreach(const Song &song, _songs)
            tempo += song.getTempo();

        return _songs.size() > 0 ? tempo/_songs.size() : 0;//if no songs, then avg is 0
    }

    QList<Song> getSongs() const { return _songs;}

    void setTitle (const QString &val) { _title = val;}
    void setYear  (const QString &val) { _year = val;}
    void setArtist(const QString &val) { _artist = val;}
    void setCover (const QImage  &val) { _cover = val;}
    void addSong  (const Song &val   ) { _songs.append(val);}

    void clear(){ _artist.clear(); _title.clear(); _year.clear(); _songs.clear(); }

    inline bool operator < (const Album &a) const { return getArtist()+getTitle() < a.getArtist()+a.getTitle();}
    inline bool operator ==(const Album &a) const { return getArtist()+getTitle() == a.getArtist()+a.getTitle();}
    inline bool operator !=(const Album &a) const { return getArtist()+getTitle() != a.getArtist()+a.getTitle();}

private:

    QString     _title, _year, _artist;
    QList<Song> _songs;    
    QImage      _cover;
};

Q_DECLARE_METATYPE(Album);


#endif // ALBUM_H
