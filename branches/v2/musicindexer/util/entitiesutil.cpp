#include "entitiesutil.h"
#include <tag.h>
#include <fileref.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mpegfile.h>
#include <tmap.h>

#include <QDirIterator>
#include <QMap>
#include <QDebug>

EntitiesUtil::EntitiesUtil()
{
}

Album EntitiesUtil::getAlbumDataFromDir(const QString &dir)
{
    Album *album;
    bool empty = true;

    QDirIterator it(dir, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString file = it.next();
        Song song;

        if (!getSongFromFile(file, song))
            continue;

        TagLib::FileRef taglibFile(file.toUtf8().data());

        if (!taglibFile.isNull() && taglibFile.tag())
        {
            TagLib::Tag *tag = taglibFile.tag();
            Song song(tag->title().toCString(), tag->album().toCString(), tag->artist().toCString(), QString::number(tag->year()), tag->genre().toCString(), "", 100, "");

            if (empty || (!album->getTitle().isEmpty() && !song.getAlbum().isEmpty() && album->getTitle() != song.getAlbum()))
            {
                album = getAlbumFromFile(file);
                empty = false;
            }

            album->addSong(song);
        }
    }

    return *album;
}

QList<Artist> EntitiesUtil::getArtistsFromSongs(const QStringList &songs)
{
    QList<Artist> artistList;
    QMap<QString, Artist*> artistMap;

    QList<Album> albums = getAlbumsFromSongs(songs);

     Q_FOREACH(const Album &album, albums)
    {
        if (!artistMap.contains(album.getArtist()))
        {
            Artist *newArtist = new Artist(album.getArtist());
            newArtist->addAlbum(album);
            artistMap.insert(album.getArtist(), newArtist);
        }
        else
        {
            artistMap.value(album.getArtist())->addAlbum(album);
        }
    }

    QMapIterator<QString, Artist*> it(artistMap);
    while (it.hasNext())
    {
        it.next();
        artistList.append(*it.value());
    }

    return artistList;
}

QList<Album> EntitiesUtil::getAlbumsFromSongs(const QStringList &songs)
{
    QList<Album> albumList;

    QMap<QString, Album*> map;
    Q_FOREACH(const QString& songFile, songs)
    {

        TagLib::FileRef tagLibFile(songFile.toUtf8().data());
        if (!tagLibFile.isNull())
        {
            TagLib::Tag *tag = tagLibFile.tag();
            Song song(tag->title().toCString(), tag->album().toCString(), tag->artist().toCString(), QString::number(tag->year()), tag->genre().toCString(), "", 100, "");

            if (!getSongFromFile(songFile, song))
                continue;

            if (map.contains(song.getArtist()+song.getAlbum()))
            {
                map.value(song.getArtist()+song.getAlbum())->addSong(song);
            }
            else
            {
                Album *album = getAlbumFromFile(songFile);
                album->addSong(song);

                map.insert(song.getArtist()+song.getAlbum(), album);
            }
        }
    }

    QMapIterator<QString, Album*> it(map);
    while (it.hasNext())
    {
        it.next();
        albumList.append(*it.value());
    }

    return albumList;
}

Album* EntitiesUtil::getAlbumFromFile(const QString &file)
{
    Album *album = NULL;

    Song song;
    if (!getSongFromFile(file, song))
        return NULL;

    album = new Album(song.getAlbum(), song.getArtist(), song.getYear());

    album->setCover(QImage(":icons/icons/cd.png"));
    album->setArtist(song.getArtist());
    album->setTitle(song.getAlbum());
    album->setYear(song.getYear());

    TagLib::FileRef tagLibFile(file.toUtf8().data());

    if (!tagLibFile.isNull() && tagLibFile.tag())
    {
        TagLib::Tag *tag = tagLibFile.tag();
        TagLib::MPEG::File *mp3  = dynamic_cast<TagLib::MPEG::File *> (tagLibFile.file());
        if (mp3 && mp3->ID3v2Tag())
        {
            //TagLib::ID3v2::FrameList list = mp3->ID3v2Tag()->frameListMap()["APIC"];
            // Las claves de FrameListMap no estan terminadas en  NULL, y tienen basura!!
            // no se puede usar el metodo de [key], hay que recorrer el mapa y
            // buscar los primeros 4 caracteres de cada clave y obtener el valor del mapa.
            TagLib::ID3v2::FrameList apicList;
            TagLib::Map<TagLib::ByteVector,TagLib::List<TagLib::ID3v2::Frame*> >::ConstIterator i = mp3->ID3v2Tag()->frameListMap().begin();
            for (; i != mp3->ID3v2Tag()->frameListMap().end();i++)
            {
                QString key = QString::fromLatin1((*i).first.data(), 4);
                //qDebug() << key;
                if (key == "APIC")
                {
                    apicList = (*i).second;
                    break;
                }
            }

            TagLib::List<TagLib::ID3v2::Frame*>::ConstIterator it = apicList.begin();
            for (; it != apicList.end();it++)
            {
                TagLib::ID3v2::Frame* frame = *it;
                //qDebug() << frame->toString().data(TagLib::String::UTF8).data();
            }

            if (apicList.size()>0)
            {
                TagLib::ID3v2::AttachedPictureFrame *cover = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*> (apicList.front());
                QImage coverArt;
                coverArt.loadFromData((const uchar*)cover->picture().data(), cover->picture().size());
                album->setCover(coverArt);
            }
        }
    }

    return album;
}

QString EntitiesUtil::getAlbumPathFromFile(const QString &filePath)
{
    //// Obtain the album path (checking if there are several cds from the same album (cd1 cd 2..)
    QString currentAlbumPath;
    TagLib::FileRef taglibFile(filePath.toUtf8().data());

    if (!taglibFile.isNull() && taglibFile.tag())
    {
        TagLib::Tag *tag = taglibFile.tag();

        currentAlbumPath = filePath.mid(0, filePath.lastIndexOf("/"));

        QDir auxDir(currentAlbumPath);
        auxDir.cdUp();
        QFileInfoList list = auxDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
        QFileInfoList sublist;

        Q_FOREACH(QFileInfo inf, list)
        {
            if (inf.isDir() && inf.absoluteFilePath() != currentAlbumPath)
            {
                auxDir.setPath(inf.absoluteFilePath());
                sublist = auxDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
                break;
            }
        }

        // if the songs from the siblings folders have the same alubm, then is another cd from the same volume
        // (we chop the CD 1 or Album 1 name from the path, leaving only the root album path
        if (!sublist.isEmpty())
        {
            TagLib::FileRef auxfile(sublist.first().absoluteFilePath().toUtf8().data());

            if (!auxfile.isNull() && auxfile.tag())
            {
                TagLib::Tag *auxtag = auxfile.tag();
                if (auxtag->album() == tag->album())
                    currentAlbumPath = currentAlbumPath.mid(0, currentAlbumPath.lastIndexOf("/"));
            }
        }
    }

    return currentAlbumPath;
}

bool EntitiesUtil::getSongFromFile(const QString &file, Song &song)
{

    TagLib::FileRef tagLibFile(file.toUtf8().data());
    if (!tagLibFile.isNull())
    {
        TagLib::Tag *tag = tagLibFile.tag();
        song = Song(tag->title().toCString(), tag->album().toCString(), tag->artist().toCString(), QString::number(tag->year()), tag->genre().toCString(), "", 100, "");

        if (tagLibFile.audioProperties())
        {
            song.setBitrate(tagLibFile.audioProperties()->bitrate());
            song.setLength(tagLibFile.audioProperties()->length());
        }

    }
    else
        return false;

    return true;
}

QPair<Song, QString> EntitiesUtil::getSongAndFileFromSongList(const QStringList &songs, const QString &artist, const QString &album, const QString &song)
{
    QPair<Song, QString> songInfo;

    Q_FOREACH(const QString& songFile, songs)
    {
        Song s;
        getSongFromFile(songFile, s);

        if (s.getArtist() == artist && s.getAlbum() == album && s.getTitle() == song)
        {
            songInfo.first  = s;
            songInfo.second = songFile;
        }
    }

    return songInfo;
}
