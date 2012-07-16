#include "similarityview.h"
#include "ui_similarityview.h"
#include <QGraphicsDropShadowEffect>

SimilarityView::SimilarityView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimilarityView)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);

    QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height());
    setMask(visibleArea);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(20);
    effect->setXOffset(0.0);
    effect->setYOffset(0.0);
    setGraphicsEffect(effect);

    ui->btSearchAlbum->setDisabled(true);

    connect(ui->listWidgetArtist, SIGNAL(clicked(QModelIndex)), this, SLOT(slotArtistSelected(QModelIndex)));
    connect(ui->btSearchArtist  , SIGNAL(clicked()), this, SLOT(slotArtistSearch()));
    connect(ui->btSearchAlbum   , SIGNAL(clicked()), this, SLOT(slotAlbumSearch()));
    connect(ui->btClose         , SIGNAL(clicked()), this, SLOT(close()));

}

SimilarityView::~SimilarityView()
{
    delete ui;
}

void SimilarityView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void SimilarityView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

bool SimilarityView::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
        setCursor(Qt::ArrowCursor);
    else if(e->type() == QEvent::MouseButtonPress)
        setCursor(Qt::ClosedHandCursor);

    return false;
}

void SimilarityView::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        close();
    }
}


void SimilarityView::setArtists(const QStringList &artists)
{
    ui->listWidgetArtist->clear();
    ui->btSearchAlbum->setDisabled(true);

     Q_FOREACH(const QString &artist, artists)
        ui->listWidgetArtist->addItem(new QListWidgetItem(artist));
}

void SimilarityView::setAlbums(const QList<QPair<QImage, QString> > &albums)
{
    ui->listWidgetAlbum->clear();
    ui->btSearchAlbum->setEnabled(true);

    for (int i=0; i<albums.size(); i++)
    {
        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(albums.at(i).first),QIcon::Normal,QIcon::On);
        ui->listWidgetAlbum->addItem(new QListWidgetItem(icon, albums.at(i).second));
   }
}

void SimilarityView::setSimilar(const QList<QPair<QImage, QString> > &items)
{
    ui->listViewSimilar->clear();

    for (int i=0; i<items.size(); i++)
    {
        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(items.at(i).first),QIcon::Normal,QIcon::On);
        ui->listViewSimilar->addItem(new QListWidgetItem(icon, items.at(i).second));
   }
}

void SimilarityView::slotArtistSelected(QModelIndex index)
{
    QString artist = index.data().toString();
    Q_EMIT artistSelected(artist);
}

void SimilarityView::slotArtistSearch()
{
    Q_EMIT artistSimilarityRequested(ui->listWidgetArtist->selectedItems().first()->text());
}
void SimilarityView::slotAlbumSearch()
{
    if (ui->listWidgetAlbum->selectedItems().size() > 0)
        Q_EMIT albumSimilarityRequested(ui->listWidgetArtist->selectedItems().first()->text(), ui->listWidgetAlbum->selectedItems().first()->text());
}
