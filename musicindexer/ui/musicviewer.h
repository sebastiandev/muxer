#ifndef MUSICVIEWER_H
#define MUSICVIEWER_H

#include <QObject>
#include "collectionview.h"
#include "similarityview.h"


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

public slots:

private slots:

    //void changeToAlbumView();
    void changeToSongView        ();
    void changeToArtistView      ();
    void showItemDetail          (QModelIndex);
    void slotLoadSongsFromAlbum  (const QString&, const QString&);
    void slotLoadAlbumsFromArtist(const QString &artist);
    void slotGetArtistSimilarity (const QString &artist);
    void slotGetAlbumSimilarity  (const QString &artist, const QString &album);



private:

    enum type{SONG, ALBUM, ARTIST, GENRE};

    CollectionView *_collectionView;
    SimilarityView *_similarityView;

    QStringList     _currentSongs;
    bool _songsLoaded;

};

#endif // MUSICVIEWER_H

