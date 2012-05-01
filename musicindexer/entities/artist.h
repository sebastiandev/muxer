#ifndef ARTIST_H
#define ARTIST_H

#include <QStringList>
#include <QMap>
#include <QMetaType>
#include "album.h"

class Artist
{
public:

    virtual ~Artist(){}
    Artist(){}
    Artist(const QString& name){ _name = name;}
    Artist(const QString& name, const QList<Album> &albums)
    {
        _name = name;
        Q_FOREACH (const Album &a, albums)
            _albums.insert(a.getTitle(), new Album(a));
    }

    QString       getName () const { return _name;}
    QList<Album> getAlbums() const
    {
        QList<Album> list;
        QMapIterator<QString, Album*> it(_albums);
        while (it.hasNext())
        {
            it.next();
            list.append(*it.value());
        }

        return list;
    }

    void setName(const QString &val){ _name = val;}
    void addAlbum(const Album &val)  { _albums.insert(val.getTitle(), new Album(val));}
    void addSong (const Song &val)
    {
        if (_albums.contains(val.getAlbum()))
        {
            _albums.value(val.getAlbum())->addSong(val);
        }
        else
        {
            Album *albumAux = new Album(val.getAlbum(), val.getArtist(), val.getYear());
            albumAux->addSong(val);
            _albums.insert(val.getAlbum(), albumAux);
        }
    }


    inline bool operator < (const Artist &a) const { return getName() <  a.getName();}
    inline bool operator ==(const Artist &a) const { return getName() == a.getName();}

private:

    QString      _name;
    QMap<QString, Album*> _albums;

};

Q_DECLARE_METATYPE(Artist);

#endif // ARTIST_H
