#ifndef ARTISTDETAIL_H
#define ARTISTDETAIL_H

#include <QWidget>
#include <QKeyEvent>
#include <QModelIndex>

namespace Ui {
    class ArtistDetail;
}

class ArtistDetail : public QWidget
{
    Q_OBJECT

public:
    explicit ArtistDetail(QWidget *parent = 0);
    ~ArtistDetail();

    void setArtistName(const QString &name);
    void setTotalAlbums(int totalAlbums);
    void setTotalSongs(int totalSongs);
    void setMostCommonGenre(const QString &genre);

    void setAlbums(const QList<QPair<QImage, QString> > albums);
    void addSong  (const QString &title, const QString &length, const QString &quality);

    void clear();

signals:

    void albumSelected(const QString&);
    void ArtistDetailClosed();

private slots:

    void slotAlbumSelected(QModelIndex index);

private:

    void keyPressEvent(QKeyEvent *e);

    Ui::ArtistDetail *ui;
};

#endif // ARTISTDETAIL_H
