#include "similarity.h"
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QPropertyAnimation>

Similarity::Similarity(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);

    qDebug() << this->width() << ", " << this->height() ;
    //QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height()+337);
    //setMask(visibleArea);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setBlurRadius(20);

    ui.wWrapper->setGraphicsEffect(shadow);

    ui.leSearch->hide();
    ui.listWidget->hide();
    ui.lwSimilar->hide();

    connect(ui.btArtist, SIGNAL(clicked()), this, SLOT(artistOptionSelected()));
    connect(ui.btAlbum, SIGNAL(clicked()), this, SLOT(albumOptionSelected()));
    connect(ui.leSearch, SIGNAL(textChanged(QString)), this, SLOT(filterList(QString)));
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemForSimilaritySelected(QListWidgetItem*)));
}

void Similarity::filterList(QString term)
{
    if (term.isEmpty())
    {
        for (int i=0; i<ui.listWidget->count(); i++)
        {
            QListWidgetItem *item = ui.listWidget->item(i);
            item->setHidden(false);
        }
    }
    else
    {
        for (int i=0; i<ui.listWidget->count(); i++)
        {
            QListWidgetItem *item = ui.listWidget->item(i);
            item->setHidden( !item->data(Qt::DisplayRole).toString().contains(term, Qt::CaseInsensitive) );
        }
    }
}

void Similarity::artistOptionSelected()
{
    ui.wInfo->hide();
    ui.leSearch->show();
    ui.listWidget->show();
    Q_EMIT artistSimilaritySelected();

    _currentView = ARTIST;
}

void Similarity::albumOptionSelected()
{
    ui.wInfo->hide();
    ui.leSearch->show();
    ui.listWidget->show();
    Q_EMIT albumSimilaritySelected();

    _currentView = ALBUM;
}

void Similarity::setArtists(const QStringList &artists)
{
    ui.listWidget->clear();

     Q_FOREACH(const QString &artist, artists)
        ui.listWidget->addItem(new QListWidgetItem(artist));
}

void Similarity::setAlbums(const QList<QPair<QImage, QString> > &albums)
{
    ui.listWidget->clear();

    for (int i=0; i<albums.size(); i++)
    {
        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(albums.at(i).first),QIcon::Normal,QIcon::On);
        ui.listWidget->addItem(new QListWidgetItem(icon, albums.at(i).second));
   }
}

void Similarity::itemForSimilaritySelected(QListWidgetItem* item)
{
    QString selection= item->data(Qt::DisplayRole).toString();

    if (_currentView == ALBUM)
    {
        Q_EMIT albumSimilarityRequested(selection.split(" - ").first(), selection.split(" - ").at(1));
    }
    else
    {
        Q_EMIT artistSimilarityRequested(selection);
    }
}

void Similarity::setSimilar(const QList<QPair<QImage, QString> > &items)
{
    ui.lwSimilar->clear();

    for (int i=0; i<items.size(); i++)
    {
        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(items.at(i).first),QIcon::Normal,QIcon::On);
        ui.lwSimilar->addItem(new QListWidgetItem(icon, items.at(i).second));
    }

    if (!ui.lwSimilar->isVisible())
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
        animation->setDuration(350);
        animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
        animation->setEndValue(QRect(this->x(), this->y(), this->width() + 400, this->height()));
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start();

        ui.lwSimilar->show();
    }
}

void Similarity::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void Similarity::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

bool Similarity::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
        setCursor(Qt::ArrowCursor);
    else if(e->type() == QEvent::MouseButtonPress)
        setCursor(Qt::ClosedHandCursor);

    return false;
}

void Similarity::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        close();
    }
}
