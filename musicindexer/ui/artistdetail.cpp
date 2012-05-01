#include "artistdetail.h"
#include "ui_artistdetail.h"
#include <QGraphicsDropShadowEffect>
#include <QDebug>

ArtistDetail::ArtistDetail(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::ArtistDetail)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);
/*
    QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height());
    setMask(visibleArea);
*/
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(20);
    effect->setXOffset(0.0);
    effect->setYOffset(0.0);
    ui->wcontainer->setGraphicsEffect(effect);

    ui->twSongs->setColumnCount(3);

    ui->twSongs->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->twSongs->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->twSongs->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);

    connect(ui->lwAlbums, SIGNAL(clicked(QModelIndex)), this, SLOT(slotAlbumSelected(QModelIndex)));

}

ArtistDetail::~ArtistDetail()
{
    delete ui;
}


void ArtistDetail::setArtistName(const QString &name)
{

}

void ArtistDetail::setTotalAlbums(int totalAlbums)
{

}

void ArtistDetail::setTotalSongs(int totalSongs)
{

}

void ArtistDetail::setMostCommonGenre(const QString &genre)
{

}

void ArtistDetail::setAlbums(const QList<QPair<QImage, QString> > albums)
{
    ui->lwAlbums->clear();

    for (int i=0; i<albums.size(); i++)
    {
        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(albums.at(i).first),QIcon::Normal,QIcon::On);
        ui->lwAlbums->addItem(new QListWidgetItem(icon, albums.at(i).second));
    }
}
void ArtistDetail::addSong(const QString &title, const QString &length, const QString &quality)
{
    ui->twSongs->setRowCount(ui->twSongs->rowCount()+1);
    ui->twSongs->setItem(ui->twSongs->rowCount()-1, 0, new QTableWidgetItem(title));
    ui->twSongs->setItem(ui->twSongs->rowCount()-1, 1, new QTableWidgetItem(length));
    ui->twSongs->setItem(ui->twSongs->rowCount()-1, 2, new QTableWidgetItem(quality));
}

void ArtistDetail::slotAlbumSelected(QModelIndex index)
{
    ui->twSongs->clearContents();
    ui->twSongs->setRowCount(0);

    QString album = index.data().toString();
    Q_EMIT albumSelected(album);
}


void ArtistDetail::clear()
{
    ui->lwAlbums->clear();
    ui->twSongs->clearContents();
}

void ArtistDetail::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        Q_EMIT ArtistDetailClosed();
        close();
    }
}
