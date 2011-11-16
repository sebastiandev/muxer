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

    _audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    _mediaObject = new Phonon::MediaObject(this);
    _metaInformationResolver = new Phonon::MediaObject(this);

    _mediaObject->setTickInterval(1000);

    connect(_mediaObject, SIGNAL(tick(qint64))                             , this, SLOT(tick(qint64)));
    //connect(_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(_mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)), this, SLOT(sourceChanged(Phonon::MediaSource)));
    //connect(_mediaObject, SIGNAL(aboutToFinish())                          , this, SLOT(aboutToFinish()));
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

    connect(_playAction , SIGNAL(triggered()), _mediaObject, SLOT(play()));
    connect(_pauseAction, SIGNAL(triggered()), _mediaObject, SLOT(pause()) );
    connect(_stopAction , SIGNAL(triggered()), _mediaObject, SLOT(stop()));

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

    ui->wSlide->layout()->addWidget(_seekSlider);
    ui->wSlide->layout()->addWidget(_timeLcd);
    ui->wSlide->layout()->addWidget(volumeLabel);
    ui->wSlide->layout()->addWidget(_volumeSlider);

    ui->wPlayback->layout()->addWidget(bar);
}

CompactPlayer::~CompactPlayer()
{
    delete ui;
}

void CompactPlayer::playSong(const QString& songFile)
{
    _mediaObject->stop();
    _mediaObject->clearQueue();

    Phonon::MediaSource source(songFile);
    _mediaObject->setCurrentSource(source);
    _mediaObject->play();
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
