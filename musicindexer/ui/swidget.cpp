#include "swidget.h"
#include <QGraphicsDropShadowEffect>

SWidget::SWidget(QWidget *parent) :
    QWidget(parent/*, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint*/)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height());
    setMask(visibleArea);

    _originalEffect = new QGraphicsDropShadowEffect();
    _originalEffect->setBlurRadius(15);
    _originalEffect->setXOffset(0.0);
    _originalEffect->setYOffset(0.0);
    _originalEffect->setColor(Qt::darkGray);

    ui.wWrapper->setGraphicsEffect(_originalEffect);

    ui.tableWidget->hide();
    ui.tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui.tableWidget->horizontalHeader()->resizeSection(0, 25);
    ui.tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui.tableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Interactive);
    ui.tableWidget->horizontalHeader()->setResizeMode(3, QHeaderView::Interactive);
    ui.tableWidget->horizontalHeader()->setResizeMode(4, QHeaderView::ResizeToContents);
    ui.tableWidget->horizontalHeader()->setResizeMode(5, QHeaderView::Interactive);
    ui.tableWidget->horizontalHeader()->setResizeMode(6, QHeaderView::ResizeToContents);

    _artistDetail = NULL;

    //connect(ui.btAlbumView , SIGNAL(clicked()), this, SIGNAL(AlbumViewSelected()));
    connect(ui.btArtistView, SIGNAL(clicked()), this, SIGNAL(ArtistViewSelected()));
    connect(ui.btGenreView , SIGNAL(clicked()), this, SIGNAL(GenresViewSelected()));
    connect(ui.btSongView  , SIGNAL(clicked()), this, SIGNAL(SongViewSelected()));
    connect(ui.btClose     , SIGNAL(clicked()), this, SLOT(close()));
    connect(ui.btMinimize  , SIGNAL(clicked()), this, SLOT(showMinimized()));

    connect(ui.listWidget  , SIGNAL(clicked(QModelIndex)), this, SIGNAL(itemSelected(QModelIndex)));
    connect(ui.tableWidget , SIGNAL(cellClicked(int,int)), this, SIGNAL(songSelected(int, int)));
}

void SWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void SWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

bool SWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
        setCursor(Qt::ArrowCursor);
    else if(e->type() == QEvent::MouseButtonPress)
        setCursor(Qt::ClosedHandCursor);

    return false;
}

void SWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        close();
    }
}

void SWidget::addArtist(const QString &artist)
{
    ui.listWidget->addItem(new QListWidgetItem(QIcon(":icons/icons/artist.png"), artist));
}

void SWidget::addAlbum(const QString &album, const QIcon &icon)
{
    ui.listWidget->addItem(new QListWidgetItem(icon, album));
}

void SWidget::addSong(const QString &artist, const QString &album, const QString &title, const QString &duration, const QString &genres, const QString &quality)
{
    if (_currentView == ARTIST && _artistDetail->isVisible())
    {
        _artistDetail->addSong(title, duration, quality);
    }
    else if (_currentView == SONG)
    {
        ui.tableWidget->setRowCount(ui.tableWidget->rowCount()+1);
        int currentRow = ui.tableWidget->rowCount()-1;

        ui.tableWidget->setItem(currentRow, 0, new QTableWidgetItem(QIcon(":icons/icons/play.png"), ""));

        ui.tableWidget->setItem(currentRow, 1, new QTableWidgetItem(title.trimmed()));
        ui.tableWidget->item(currentRow, 1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.tableWidget->setItem(currentRow, 2, new QTableWidgetItem(artist.trimmed()));
        ui.tableWidget->item(currentRow, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.tableWidget->setItem(currentRow, 3, new QTableWidgetItem(album.trimmed()));
        ui.tableWidget->item(currentRow, 3)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.tableWidget->setItem(currentRow, 4, new QTableWidgetItem(duration.trimmed()));
        ui.tableWidget->item(currentRow, 4)->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        ui.tableWidget->setItem(currentRow, 5, new QTableWidgetItem(genres.trimmed()));
        ui.tableWidget->item(currentRow, 5)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.tableWidget->setItem(currentRow, 6, new QTableWidgetItem(quality.trimmed()));
        ui.tableWidget->item(currentRow, 6)->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    }
}

void SWidget::clear()
{
    ui.listWidget->clear();
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);
}

void SWidget::showSongView()
{
    ui.listWidget->hide();
    ui.tableWidget->show();
    show();
    activateWindow();
    _currentView = SONG;
}

void SWidget::showArtistView()
{
    ui.tableWidget->hide();
    ui.listWidget->show();
    show();
    activateWindow();
    _currentView = ARTIST;
}

void SWidget::showAlbumView()
{
    ui.listWidget->show();
    show();
    activateWindow();
    _currentView = ALBUM;
}

void SWidget::showGenreView()
{
    ui.listWidget->hide();
    show();
    activateWindow();
    _currentView = GENRE;
}

void SWidget::showArtistDetail(const QString &artist, const QList<QPair<QImage, QString> > &albums)
{
    if (_artistDetail == NULL)
    {
        _artistDetail = new ArtistDetail();
        _artistDetail->setWindowFlags(_artistDetail->windowFlags() | Qt::Tool);

        _artistDetail->move(this->x() + ((this->width() - _artistDetail->width())/2), this->y() + ((this->height() - _artistDetail->height()))/2);
        connect(_artistDetail, SIGNAL(albumSelected(const QString&)), this, SLOT(slotAlbumSelected(const QString&)));
        connect(_artistDetail, SIGNAL(ArtistDetailClosed()), this, SLOT(restoreView()));
    }

    _blurEffect = new QGraphicsBlurEffect();
    _blurEffect->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
    ui.wWrapper->setGraphicsEffect(_blurEffect);

    _artistDetail->clear();
    _artistDetail->setAlbums(albums);
    _artistDetail->show();
}

void SWidget::slotAlbumSelected(const QString &album)
{
    emit albumSelectedFromArtistDetail(ui.listWidget->selectedItems().first()->text(), album);
}

void SWidget::restoreView()
{
    _originalEffect = new QGraphicsDropShadowEffect();
    _originalEffect->setBlurRadius(20);
    _originalEffect->setXOffset(0.0);
    _originalEffect->setYOffset(0.0);

    ui.wWrapper->setGraphicsEffect(_originalEffect);
}

