#include "musicmanager.h"

#include <iostream>
#include <assert.h>
#include <QDebug>
#include <QDir>
#include <QDirIterator>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include "util/QFileUtils.h"
#include "util/entitiesutil.h"
#include "queryparser.h"
#include "entities/song.h"
#include "configuration/ConfigurationManager.h"
#include "database/IndexerException.h"
#include "logging/LoggerManager.h"

MusicManager::~MusicManager()
{
    _db.close();
}

MusicManager::MusicManager(SongIndexer songIndexer, SongFinder songFinder)
{
    _indexer = songIndexer;
    _finder  = songFinder;

    try
    {
        _db = Xapian::WritableDatabase(ConfigurationManager::GetString("musicdb").toStdString(), Xapian::DB_CREATE_OR_OPEN);
        _indexer.setDataBase(_db);
        _finder.setDataBase(_db);
    }
    catch (const Xapian::Error &e)
    {
        std::cout << e.get_description() << std::endl;
    }
}

MusicManager& MusicManager::manager()
{
    static MusicManager instance = MusicManager(SongIndexer(), SongFinder());//allocate only once
    return instance;
}

void MusicManager::addSongsFromDirectory(QString dirPath)
{
    assert(!dirPath.isEmpty());

    Album currentAlbum;
    QString file, currentAlbumPath;
    Song song;

    QDirIterator it(dirPath, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        file = it.next();
        TagLib::FileRef taglibFile(file.toUtf8().data());

        if (!taglibFile.isNull() && taglibFile.tag())
        {
            qDebug() << "Indexing file: " << file;

            TagLib::Tag *tag = taglibFile.tag();
            song = Song(tag->title().toCString(), tag->album().toCString(), tag->artist().toCString(), QString::number(tag->year()), tag->genre().toCString(), "", 100, "");

            try
            {
                _indexer.index(file, song);
            }
            catch (IndexerException &ie)
            {
                qDebug() << "Error '" << ie.getMsg() << "' " << "indexing file: " << ie.getFile();
                LoggerManager::LogDebug("[Music Manager] Error indexing file: " + ie.getFile() + ". Cause: " + ie.getMsg());
                continue;
            }

            if (song.getAlbum()  != currentAlbum.getTitle())
            {
                if (!currentAlbum.getTitle().isEmpty())
                {                    
                    SimilarityManager::manager().addAlbum(currentAlbumPath, currentAlbum);
                    currentAlbum.clear();
                }

                currentAlbumPath = EntitiesUtil::getAlbumPathFromFile(file);

                qDebug () << "Album path: " << currentAlbumPath;

                currentAlbum.setArtist(song.getArtist());
                currentAlbum.setTitle(song.getAlbum());
                currentAlbum.setYear(song.getYear());
                currentAlbum.addSong(song);
            }
            else
            {
                currentAlbum.addSong(song);
            }
        }
    }

    // add last album
    SimilarityManager::manager().addAlbum(currentAlbumPath, currentAlbum);
    currentAlbum.clear();
}

void MusicManager::addSongs(QStringList songsPath)
{
    foreach(const QString &song, songsPath)
        addSongsFromDirectory(song);
}

void MusicManager::addSong(QString songPath)
{

}

QList<Album> MusicManager::getAlbumsByArtist(const QString &artist)
{
    QStringList songs = _finder.searchExactTerms(artist.split(" "));
    return EntitiesUtil::getAlbumsFromSongs(songs);
}

Artist MusicManager::getArtist(const QString &artistName)
{
    QStringList songs = _finder.searchExactTerms(artistName.split(" "));
    return EntitiesUtil::getArtistsFromSongs(songs).first();
}

QStringList MusicManager::search(QString query)
{
    QStringList expandedQuery = QueryParser::parser().expandQuery(query);
    return _finder.search(expandedQuery);
}

QStringList MusicManager::getAllSongs()
{
    return _finder.allSongs();
}
