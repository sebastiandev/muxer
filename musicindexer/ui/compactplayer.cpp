#include "compactplayer.h"
#include "ui_compactplayer.h"
#include <QToolBar>
#include <QLabel>
#include <QTime>

CompactPlayer::CompactPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompactPlayer)
{
    ui->setupUi(this);

    _playlist = new CompactPlaylist();
    _playlist->hide();

    _audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    _mediaObject = new Phonon::MediaObject(this);
    _metaInformationResolver = new Phonon::MediaObject(this);

    _mediaObject->setTickInterval(1000);

    connect(_playlist, SIGNAL(playSong(Song,QString)), this, SLOT(playSong(Song,QString)));

    connect(_mediaObject, SIGNAL(tick(qint64))                             , this, SLOT(tick(qint64)));
    //connect(_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(_mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)), this, SLOT(sourceChanged(Phonon::MediaSource)));
    connect(_mediaObject, SIGNAL(aboutToFinish())                          , this, SLOT(aboutToFinish()));
    //connect(_metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));

    Phonon::createPath(_mediaObject, _audioOutput);

    _playAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
    _playAction->setShortcut(tr("Ctrl+P"));
    _playAction->setDisabled(false);

    _pauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
    _pauseAction->setShortcut(tr("Ctrl+A"));
    _pauseAction->setDisabled(false);

    _stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
    _stopAction->setShortcut(tr("Ctrl+S"));
    _stopAction->setDisabled(false);

    QToolBar *bar = new QToolBar;

    bar->addAction(_playAction);
    bar->addAction(_pauseAction);
    bar->addAction(_stopAction);
    bar->setStyleSheet("QToolBar { border: 0px solid grey; border-radius: 6px; background-color: transparent;}");

    _seekSlider = new Phonon::SeekSlider(this);
    _seekSlider->setMediaObject(_mediaObject);
    _seekSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    _seekSlider->setStyleSheet( "QSlider::groove:horizontal {"
                                "border: 1px solid #999999;"
                                "height: 6px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */"
                                "background-color: rgb(110, 110, 110); /*rgba(160, 235, 255, 180);*/}"
                                "QSlider::handle:horizontal {"
                                "background-color: grey;"
                                "border: 0.5px solid #5c5c5c;"
                                "width: 10px;"
                                "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                "border-radius: 2px;}"
                                );
    _volumeSlider = new Phonon::VolumeSlider(this);
    _volumeSlider->setAudioOutput(_audioOutput);
    _volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QLabel *volumeLabel = new QLabel;
    volumeLabel->setPixmap(QPixmap("images/volume.png"));

    QPalette palette;
    palette.setBrush(QPalette::Light, Qt::darkGray);

    _timeLcd = new QLCDNumber;
    _timeLcd->setPalette(palette);
    _timeLcd->setStyleSheet("border: 0px solid grey; border-radius: 4px; ");

    _btPlaylist = new QPushButton();
    _playlist->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    _btPlaylist->setStyleSheet("QPushButton { "
                                 "image: url(:/icons/icons/playlist.png); "
                                 "margin-right: 3px; border: 0px solid grey;"
                                 "border-top-right-radius: 4px; "
                                 "border-bottom-right-radius: 4px; "
                                 "background-color: transparent;}"

                                 "QPushButton::hover{ background-color: white}");

    ui->wSlide->layout()->addWidget(_seekSlider);
    ui->wSlide->layout()->addWidget(_timeLcd);
    ui->wSlide->layout()->addWidget(volumeLabel);
    ui->wSlide->layout()->addWidget(_volumeSlider);
    ui->wSlide->layout()->addWidget(_btPlaylist);

    ui->wPlayback->layout()->addWidget(bar);

    connect(_playAction , SIGNAL(triggered()), _mediaObject, SLOT(play()));
    connect(_pauseAction, SIGNAL(triggered()), _mediaObject, SLOT(pause()) );
    connect(_stopAction , SIGNAL(triggered()), _mediaObject, SLOT(stop()));
    connect(_btPlaylist,  SIGNAL(clicked()) ,  this, SLOT(tooglePlaylist()));
}

CompactPlayer::~CompactPlayer()
{
    delete ui;
}

void CompactPlayer::tooglePlaylist()
{
    if (_playlist->isVisible())
        _playlist->hide();
    else
    {
        QPoint globalPos = this->mapToGlobal(this->pos());
        _playlist->setGeometry(globalPos.x(), globalPos.y() + this->geometry().height() - 5, this->width(), 200);
        _playlist->show();
    }
}

void CompactPlayer::addSong(const Song &song, const QString &songFile)
{
    _playlist->addSong(song, songFile);
}

void CompactPlayer::playSong(const Song &song, const QString& songFile)
{
    _mediaObject->stop();
    _mediaObject->clearQueue();

    Phonon::MediaSource source(songFile);
    _mediaObject->setCurrentSource(source);
    _mediaObject->play();

    // a new song is played, then add it to playlist and update current song on the playlist
    if (!_playlist->contains(song))
    {
        _playlist->addSong(song, songFile);
        _playlist->setCurrentSong(_playlist->size());
    }
}

int CompactPlayer::songsInQueue()
{
    // if a song is been played we consider the song to be in the queue
    int putPLayingAsQueue = _mediaObject->state() == Phonon::PlayingState ? 1 : 0;
    return (_playlist->size() - _playlist->currentSong() + putPLayingAsQueue);
}

void CompactPlayer::aboutToFinish()
{
    _playlist->nextSong();
}

void CompactPlayer::tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
    _timeLcd->display(displayTime.toString("mm:ss"));
}

void CompactPlayer::sourceChanged(const Phonon::MediaSource &source)
{
    //musicTable->selectRow(sources.indexOf(source));
    _timeLcd->display("00:00");
}

void CompactPlayer::updatePosition(QPoint pos)
{
    if (_playlist->isVisible())
    {
        QPoint globalPos = this->mapToGlobal(this->pos());
        _playlist->move(globalPos.x(), globalPos.y() + this->geometry().height() - 5);
    }
}

void CompactPlayer::close()
{
    _mediaObject->clearQueue();
    _mediaObject->stop();
    _playlist->close();
}
