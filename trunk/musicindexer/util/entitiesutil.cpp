#include "entitiesutil.h"
#include "taglib/tag.h"
#include "taglib/fileref.h"
#include "taglib/id3v2tag.h"
#include "taglib/attachedpictureframe.h"
#include "taglib/mpegfile.h"
#include "taglib/tmap.h"

#include <QDirIterator>
#include <QMap>
#include <QDebug>

EntitiesUtil::EntitiesUtil()
{
}

Album EntitiesUtil::getAlbumDataFromDir(const QString &dir)
{
    Album album;
    bool empty = true;

    QDirIterator it(dir, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString file = it.next();
        TagLib::FileRef taglibFile(file.toUtf8().data());

        if (!taglibFile.isNull() && taglibFile.tag())
        {
            TagLib::Tag *tag = taglibFile.tag();
            Song song(tag->title().toCString(), tag->album().toCString(), tag->artist().toCString(), QString::number(tag->year()), tag->genre().toCString(), "", 100, "");
            album.addSong(song);

            if (empty)
            {
                album.setCover(QImage(":icons/icons/cd.png"));
                album.setArtist(song.getArtist());
                album.setTitle(song.getAlbum());
                album.setYear(song.getYear());
                empty = false;
            }

            TagLib::MPEG::File *mp3  = dynamic_cast<TagLib::MPEG::File *> (taglibFile.file());
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
                }

                if (apicList.size()>0)
                {
                    TagLib::ID3v2::AttachedPictureFrame *cover = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*> (apicList.front());
                    QImage coverArt;
                    coverArt.loadFromData((const uchar*)cover->picture().data(), cover->picture().size());
                    album.setCover(coverArt);
                }
            }
        }
    }

    return album;
}

QList<Artist> EntitiesUtil::getArtistsFromSongs(const QStringList &songs)
{
    QList<Artist> artistList;
    QMap<QString, Artist*> artistMap;

    QList<Album> albums = getAlbumsFromSongs(songs);

    foreach (const Album &album, albums)
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
    foreach(const QString& songFile, songs)
    {
        TagLib::FileRef tagLibFile(songFile.toUtf8().data());
        if (!tagLibFile.isNull())
        {
            TagLib::Tag *tag = tagLibFile.tag();
            Song song(tag->title().toCString(), tag->album().toCString(), tag->artist().toCString(), QString::number(tag->year()), tag->genre().toCString(), "", 100, "");

            if (tagLibFile.audioProperties())
            {
                song.setBitrate(tagLibFile.audioProperties()->bitrate());
                song.setLength(tagLibFile.audioProperties()->length());
            }

            if (map.contains(song.getArtist()+song.getAlbum()))
            {
                map.value(song.getArtist()+song.getAlbum())->addSong(song);
            }
            else
            {
                Album *album = new Album(song.getAlbum(), song.getArtist(), song.getYear());
                album->setCover(QImage(":icons/icons/cd.png"));

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
