#ifndef MUSICVIEWER_H
#define MUSICVIEWER_H

#include <QObject>
#include "collectionview.h"
#include "similarityview.h"
#include "swidget.h"
#include "similarity.h"
#include "entities/song.h"


class MusicViewer : public QObject
{
    Q_OBJECT
public:
    explicit MusicViewer(QObject *parent = 0);


    void setMusicCollection  (const QStringList &collection);
    void addSongsToCollection(const QStringList &collection);

    void showSongs           (const QStringList &songs);
    void showSimilarityScreen(const QStringList &songs);


signals:

    void playSong(const Song&, const QString&);

public slots:

    void close();

private slots:

    //void changeToAlbumView();
    void changeToSongView        ();
    void changeToArtistView      ();
    void showItemDetail          (QModelIndex);
    void slotLoadSongsFromAlbum  (const QString&, const QString&);
    void slotLoadAlbumsFromArtist(const QString &artist);
    void slotGetArtistSimilarity (const QString &artist);
    void slotGetAlbumSimilarity  (const QString &artist, const QString &album);
    void loadArtistForSimilarity ();
    void loadAlbumForSimilarity  ();
    void slotGetSongDataAndPlay  (const QString &artist, const QString &album, const QString &song);



private:

    enum type{SONG, ALBUM, ARTIST, GENRE};

    SWidget    *_collectionView;
    Similarity *_similarityView;

    QStringList     _currentSongs;
    bool _songsLoaded;

};

#endif // MUSICVIEWER_H

