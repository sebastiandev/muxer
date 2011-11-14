#ifndef SWIDGET_H
#define SWIDGET_H

#include "ui_swidget.h"
#include <QModelIndex>
#include <QIcon>
#include <QMouseEvent>
#include <QTableView>
#include "artistdetail.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>


class SWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SWidget(QWidget *parent = 0);

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

signals:

    void itemSelected(QModelIndex);
    void songSelected(int, int);
    void SongViewSelected();
    void ArtistViewSelected();
    void AlbumViewSelected();
    void GenreViewSelected();
    void albumSelectedFromArtistDetail(const QString&, const QString&);

private slots:

    void slotAlbumSelected(const QString &album);
    void restoreView();

private:

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);

    Ui::SWidget ui;

    QPoint       m_dragPosition;
    ArtistDetail *_artistDetail;

    int _currentView;

    QGraphicsDropShadowEffect *_originalEffect;
    QGraphicsBlurEffect       *_blurEffect;

};

#endif // SWIDGET_H
