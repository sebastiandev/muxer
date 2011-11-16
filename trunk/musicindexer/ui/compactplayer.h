#ifndef COMPACTPLAYER_H
#define COMPACTPLAYER_H

#include <QWidget>
#include <QAction>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>
#include <QLCDNumber>

namespace Ui {
    class CompactPlayer;
}

class CompactPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit CompactPlayer(QWidget *parent = 0);
    ~CompactPlayer();

public slots:

    void playSong(const QString& songFile);

private slots:

    void tick(qint64 time);
    void sourceChanged(const Phonon::MediaSource &source);


private:
    Ui::CompactPlayer *ui;

    Phonon::AudioOutput  *_audioOutput;
    Phonon::MediaObject  *_mediaObject;
    Phonon::MediaObject  *_metaInformationResolver;
    Phonon::SeekSlider   *_seekSlider;
    Phonon::VolumeSlider *_volumeSlider;

    QLCDNumber *_timeLcd;

    QAction *_playAction;
    QAction *_pauseAction;
    QAction *_stopAction;

};

#endif // COMPACTPLAYER_H
