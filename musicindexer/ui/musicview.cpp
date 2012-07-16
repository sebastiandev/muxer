#include "musicview.h"
#include "ui_musicview.h"
#include <QGraphicsDropShadowEffect>

MusicView::MusicView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicView)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    effect->setXOffset(0.0);
    effect->setYOffset(4.0);

    ui->wToolBar->setGraphicsEffect(effect);

    ui->wInfo->hide();

    songView = new SongView(ui->wView);

    songView->addSong("Richie Kotzen", "24 Hours", "OMG", "3:23", "Rock, Soul", "320");
    songView->addSong("Richie Kotzen", "24 Hours", "Help Me", "4:03", "Rock, Soul", "320");
    songView->addSong("Richie Kotzen", "24 Hours", "24 Hours", "5:30", "Rock, Soul", "320");
    songView->addSong("Richie Kotzen", "24 Hours", "Bad Situation", "3:23", "Rock, Soul", "320");
}

MusicView::~MusicView()
{
    delete ui;
}

void MusicView::showSongs(const QList<Song> &songs)
{
    songView->addSongs(songs);
}
