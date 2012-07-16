#ifndef COLLECTIONVIEW_H
#define COLLECTIONVIEW_H

#include <QWidget>
#include <QModelIndex>
#include <QIcon>
#include <QMouseEvent>
#include <QTableView>
#include "artistdetail.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

namespace Ui {
    class CollectionView;
}

class CollectionView : public QWidget
{
    Q_OBJECT

public:
    explicit CollectionView(QWidget *parent = 0);
    ~CollectionView();

    enum viewType{ SONG, ALBUM, ARTIST, GENRE};

    void addArtist(const QString &artist);
    void addAlbum (const QString &album, const QIcon &icon=QIcon());
    void addSong  (const QString &artist, const QString &album, const QString &title, const QString &duration, const QString &genres=QString(), const QString &quality=QString());

    void showSongView();
    void showArtistView();
    void showAlbumView();
    void showGenreView();

    void showArtistDetail(const QString &artist, const QList<QPair<QImage, QString> > &albums);

    int  currentView(){ return _currentView;}

    void clear();

Q_SIGNALS:

    void itemSelected(QModelIndex);
    void songSelected(int, int);
    void SongViewSelected();
    void ArtistViewSelected();
    void AlbumViewSelected();
    void GenreViewSelected();
    void albumSelectedFromArtistDetail(const QString&, const QString&);

private Q_SLOTS:

    void slotAlbumSelected(const QString &album);
    void restoreView();

private:

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);


    QPoint       m_dragPosition;

    Ui::CollectionView *ui;
    ArtistDetail *_artistDetail;

    int _currentView;

    QGraphicsDropShadowEffect *_originalEffect;
    QGraphicsBlurEffect       *_blurEffect;
};

#endif // COLLECTIONVIEW_H
