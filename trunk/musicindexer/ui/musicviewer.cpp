#include "musicviewer.h"

#include "entities/album.h"
#include "entities/artist.h"
#include "entities/song.h"
#include "util/entitiesutil.h"
#include "controller/similaritymanager.h"


MusicViewer::MusicViewer(QObject *parent) :
    QObject(parent)
{
    _collectionView = NULL;
    _similarityView = NULL;
    _songsLoaded = false;
}

void MusicViewer::setMusicCollection(const QStringList &collection)
{

}

void MusicViewer::addSongsToCollection(const QStringList &collection)
{

}

void MusicViewer::showSongs(const QStringList &songs)
{
    _currentSongs = songs;
    _songsLoaded  = false;

    if (_collectionView == NULL)
    {
        _collectionView = new SWidget();
        _collectionView->setWindowTitle("Songs View");
        if (parent() != NULL)
        {
            QWidget *parentWidget = (QWidget*)parent();
            _collectionView->move(parentWidget->x()- ((_collectionView->width() - parentWidget->width())/2), parentWidget->y() + parentWidget->height());
        }

        connect(_collectionView, SIGNAL(itemSelected(QModelIndex)), this, SLOT(showItemDetail(QModelIndex)));
        connect(_collectionView, SIGNAL(songSelected(const QString&, const QString&, const QString&)), this, SLOT(slotGetSongDataAndPlay(const QString&, const QString&, const QString&)));
        connect(_collectionView, SIGNAL(SongViewSelected())  , this, SLOT(changeToSongView()));
        //connect(_collectionView, SIGNAL(AlbumViewSelected()) , this, SLOT(changeToAlbumView()));
        connect(_collectionView, SIGNAL(ArtistViewSelected()), this, SLOT(changeToArtistView()));
        //connect(_collectionView, SIGNAL(GenreViewSelected()) , this, SLOT(changeToGenreView()));
        connect(_collectionView, SIGNAL(albumSelectedFromArtistDetail(const QString&, const QString&)), this, SLOT(slotLoadSongsFromAlbum(const QString&, const QString&)));
    }

    _collectionView->clear();
    _collectionView->showArtistView();

    foreach (const Artist &a, EntitiesUtil::getArtistsFromSongs(_currentSongs))
        _collectionView->addArtist(a.getName());

}

void MusicViewer::changeToArtistView()
{
    _collectionView->showArtistView();
}

void MusicViewer::changeToSongView()
{
     _collectionView->showSongView();

     if (!_songsLoaded)
     {
        foreach (const Album &album, EntitiesUtil::getAlbumsFromSongs(_currentSongs))
            foreach(const Song &song, album.getSongs())
            {
                QString length = QString::number(song.getLength());
                length.insert(length.size()-2, ":");
                _collectionView->addSong(song.getArtist(), song.getAlbum(), song.getTitle(), length, song.getGenres().join(","), QString::number(song.getBitrate()));
            }

        _songsLoaded = true;
     }
}

void MusicViewer::showItemDetail(QModelIndex index)
{
    QString item = index.data().toString();
    if (_collectionView->currentView() == CollectionView::ARTIST)
    {
        foreach(const Artist &artist, EntitiesUtil::getArtistsFromSongs(_currentSongs))
        {
            if (artist.getName() == item)
            {
                //show details of this artist
                QList<QPair<QImage, QString> > albums;
                foreach (const Album &album, artist.getAlbums())
                    albums.append(QPair<QImage, QString> (album.getCover(), album.getTitle()));

                _collectionView->showArtistDetail(artist.getName(), albums);

            }
        }
    }
    else if (_collectionView->currentView() == CollectionView::ALBUM)
    {
        foreach(const Album &album, EntitiesUtil::getAlbumsFromSongs(_currentSongs))
        {
            if (album.getArtist()+album.getTitle() == item)
            {
                 //show details of this album
            }
        }
    }
}

void MusicViewer::slotLoadSongsFromAlbum(const QString& artist, const QString& album)
{
    foreach (const Album &a, EntitiesUtil::getAlbumsFromSongs(_currentSongs))
    {
        if (a.getArtist() == artist && a.getTitle() == album)
        {
            foreach (const Song &song, a.getSongs())
            {
                QString length = QString::number(song.getLength());
                length.insert(length.size()-2, ":");
                _collectionView->addSong(song.getArtist(),song.getAlbum(), song.getTitle(), length, song.getGenres().join(","), QString::number(song.getBitrate()));
            }
        }
    }
}

void MusicViewer::showSimilarityScreen(const QStringList &songs)
{
    _currentSongs = songs;

    if (_similarityView == NULL)
    {
        _similarityView = new Similarity();
        _similarityView->setWindowFlags(_similarityView->windowFlags() | Qt::Tool);
        _similarityView->setWindowTitle("Similarity");

        connect(_similarityView, SIGNAL(artistSimilaritySelected()), this, SLOT(loadArtistForSimilarity()));
        connect(_similarityView, SIGNAL(albumSimilaritySelected()) , this, SLOT(loadAlbumForSimilarity()));
        connect(_similarityView, SIGNAL(artistSimilarityRequested(const QString&))              , this, SLOT(slotGetArtistSimilarity(const QString&)));
        connect(_similarityView, SIGNAL(albumSimilarityRequested(const QString&,const QString&)), this, SLOT(slotGetAlbumSimilarity(const QString&, const QString&)));

        if (parent() != NULL)
        {
            QWidget *parentWidget = (QWidget*)parent();
            _similarityView->move(parentWidget->x()- ((_similarityView->width() - parentWidget->width())/2), parentWidget->y() + parentWidget->height());
        }
    }

    _similarityView->show();
}

void MusicViewer::loadArtistForSimilarity()
{
    QStringList artists;
    foreach (const Artist &artist, EntitiesUtil::getArtistsFromSongs(_currentSongs))
    {
        artists << artist.getName();
    }

    _similarityView->setArtists(artists);
}

void MusicViewer::loadAlbumForSimilarity()
{
    QList<QPair<QImage, QString> > albums;
    foreach (const Album &album, EntitiesUtil::getAlbumsFromSongs(_currentSongs))
    {
        albums.append(QPair<QImage, QString>(album.getCover(), album.getArtist() + " - " + album.getTitle()));
    }

    _similarityView->setAlbums(albums);
}

void MusicViewer::slotLoadAlbumsFromArtist(const QString &artist)
{
    QList<QPair<QImage, QString> > albums;
    foreach (const Artist &a, EntitiesUtil::getArtistsFromSongs(_currentSongs))
    {
        if (a.getName() == artist)
        {
            foreach(const Album &album, a.getAlbums())
                albums.append(QPair<QImage, QString>(album.getCover(), album.getTitle()));

            break;
        }
    }

    _similarityView->setAlbums(albums);
}

void MusicViewer::slotGetArtistSimilarity(const QString &artist)
{
    QList<Artist> similarArtist;
    QList<QPair<QImage, QString> > similar;

    foreach (const Artist &a, EntitiesUtil::getArtistsFromSongs(_currentSongs))
    {
        if (a.getName() == artist)
        {
            similarArtist = SimilarityManager::manager().getSimilarArtists(a);
            break;
        }
    }

    foreach (const Artist &a, similarArtist)
        similar.append(QPair<QImage, QString>(QImage(":icons/icons/artist.png"), a.getName()));

    _similarityView->setSimilar(similar);
}

void MusicViewer::slotGetAlbumSimilarity(const QString &artist, const QString &album)
{
    QList<Album> similarAlbums;
    QList<QPair<QImage, QString> > similar;

    foreach (const Album &a, EntitiesUtil::getAlbumsFromSongs(_currentSongs))
    {
        if (a.getArtist() == artist && a.getTitle() == album)
        {
            similarAlbums = SimilarityManager::manager().getSimilarAlbums(a);
            break;
        }
    }

    foreach (const Album &a, similarAlbums)
        similar.append(QPair<QImage, QString>(a.getCover(), a.getArtist()+" - "+a.getTitle()));

    _similarityView->setSimilar(similar);
}

void MusicViewer::slotGetSongDataAndPlay(const QString &artist, const QString &album, const QString &song)
{
    QPair<Song, QString> songFile = EntitiesUtil::getSongAndFileFromSongList(_currentSongs, artist, album, song);

    emit playSong(songFile.first, songFile.second);
}

void MusicViewer::close()
{
    if (_similarityView)
        _similarityView->close();

    if (_collectionView)
        _collectionView->close();
}
