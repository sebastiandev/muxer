#include "collectionview.h"
#include "ui_collectionview.h"

CollectionView::CollectionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CollectionView)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height());
    setMask(visibleArea);

    _originalEffect = new QGraphicsDropShadowEffect();
    _originalEffect->setBlurRadius(20);
    _originalEffect->setXOffset(0.0);
    _originalEffect->setYOffset(0.0);
    ui->container->setGraphicsEffect(_originalEffect);

    ui->tableWidget->hide();
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

    _artistDetail = NULL;

    connect(ui->btAlbumView , SIGNAL(clicked()), this, SIGNAL(AlbumViewSelected()));
    connect(ui->btArtistView, SIGNAL(clicked()), this, SIGNAL(ArtistViewSelected()));
    connect(ui->btGenreView , SIGNAL(clicked()), this, SIGNAL(GenresViewSelected()));
    connect(ui->btSongView  , SIGNAL(clicked()), this, SIGNAL(SongViewSelected()));
    connect(ui->btClose     , SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btHide      , SIGNAL(clicked()), this, SLOT(showMinimized()));

    connect(ui->listWidget  , SIGNAL(clicked(QModelIndex)), this, SIGNAL(itemSelected(QModelIndex)));
    connect(ui->tableWidget , SIGNAL(cellClicked(int,int)), this, SIGNAL(songSelected(int, int)));

}


CollectionView::~CollectionView()
{
    delete ui;
}

void CollectionView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void CollectionView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

bool CollectionView::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
        setCursor(Qt::ArrowCursor);
    else if(e->type() == QEvent::MouseButtonPress)
        setCursor(Qt::ClosedHandCursor);

    return false;
}

void CollectionView::addArtist(const QString &artist)
{
    ui->listWidget->addItem(new QListWidgetItem(QIcon(":icons/icons/artist.png"), artist));
}

void CollectionView::addAlbum(const QString &album, const QIcon &icon)
{
    ui->listWidget->addItem(new QListWidgetItem(icon, album));
}

void CollectionView::addSong(const QString &artist, const QString &album, const QString &title, const QString &duration, const QString &genres, const QString &quality)
{
    if (_currentView == ARTIST && _artistDetail->isVisible())
    {
        _artistDetail->addSong(title, duration, quality);
    }
    else if (_currentView == SONG)
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(title));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(artist));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(album));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(duration));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(genres));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 5, new QTableWidgetItem(quality));
    }
}

void CollectionView::clear()
{
    ui->listWidget->clear();
    ui->tableWidget->clearContents();
}

void CollectionView::showSongView()
{
    ui->listWidget->hide();
    ui->tableWidget->show();
    show();
    activateWindow();
    _currentView = SONG;
}

void CollectionView::showArtistView()
{
    ui->tableWidget->hide();
    ui->listWidget->show();
    show();
    activateWindow();
    _currentView = ARTIST;
}

void CollectionView::showAlbumView()
{
    ui->listWidget->show();
    show();
    activateWindow();
    _currentView = ALBUM;
}

void CollectionView::showGenreView()
{
    ui->listWidget->hide();
    show();
    activateWindow();
    _currentView = GENRE;
}

void CollectionView::showArtistDetail(const QString &artist, const QList<QPair<QImage, QString> > &albums)
{
    if (_artistDetail == NULL)
    {
        _artistDetail = new ArtistDetail();
        //_artistDetail->setWindowFlags(_artistDetail->windowFlags() | Qt::Tool);

        _artistDetail->move(this->x() + ((this->width() - _artistDetail->width())/2), this->y() + ((this->height() - _artistDetail->height()))/2);
        connect(_artistDetail, SIGNAL(albumSelected(const QString&)), this, SLOT(slotAlbumSelected(const QString&)));
        connect(_artistDetail, SIGNAL(ArtistDetailClosed()), this, SLOT(restoreView()));
    }

    _blurEffect = new QGraphicsBlurEffect();
    _blurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
    ui->container->setGraphicsEffect(_blurEffect);

    _artistDetail->clear();
    _artistDetail->setAlbums(albums);
    _artistDetail->show();
}

void CollectionView::slotAlbumSelected(const QString &album)
{
    emit albumSelectedFromArtistDetail(ui->listWidget->selectedItems().first()->text(), album);
}

void CollectionView::restoreView()
{
    _originalEffect = new QGraphicsDropShadowEffect();
    _originalEffect->setBlurRadius(20);
    _originalEffect->setXOffset(0.0);
    _originalEffect->setYOffset(0.0);

    ui->container->setGraphicsEffect(_originalEffect);
}

void CollectionView::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        close();
    }
}
