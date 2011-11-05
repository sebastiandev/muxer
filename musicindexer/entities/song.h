#ifndef SONG_H
#define SONG_H

#include <QStringList>
#include <QMetaType>

class Song
{
public:

    ~Song(){}
    Song(){}

    Song(QString title,
         QString album,
         QString artist,
         QString year,
         QString genres,
         QString key,
         int     tempo,
         QString type)
    {
        _title  = title.trimmed();
        _album  = album.trimmed();
        _artist = artist.trimmed();
        _year   = year.trimmed();
        _genres  = genres.split(",");
        _key    = key.trimmed();
        _tempo  = tempo;
        _type   = type.trimmed();
    }

    bool hasTitle () const { return !_title.isEmpty(); }
    bool hasAlbum () const { return !_album.isEmpty(); }
    bool hasArtist() const { return !_artist.isEmpty();}
    bool hasYear  () const { return !_year.isEmpty();  }
    bool hasGenres() const { return !_genres.isEmpty(); }
    bool hasKey   () const { return !_key.isEmpty();   }
    bool hasTempo () const { return _tempo != 0; }
    bool hasType  () const { return !_type.isEmpty();  }

    QString getTitle () const { return _title; }
    QString getAlbum () const { return _album; }
    QString getArtist() const { return _artist;}
    QString getYear  () const { return _year;  }
    QString getKey   () const { return _key;   }
    int     getTempo () const { return _tempo; }
    QString getType  () const { return _type;  }
    QStringList getGenres() const { return _genres; }
    int     getBitrate() const { return _bitrate;}
    int     getLength () const { return  _length;}

    void    setBitrate(int val) { _bitrate = val;}
    void    setLength (int val) { _length  = val;}

private:

     QString     _title, _album, _artist, _year , _key, _type;
     QStringList _genres;
     int         _tempo, _bitrate, _length;

};

Q_DECLARE_METATYPE(Song);

#endif // SONG_H
