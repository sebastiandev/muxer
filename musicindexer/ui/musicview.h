#ifndef MUSICVIEW_H
#define MUSICVIEW_H

#include <QWidget>
#include "songview.h"

namespace Ui {
class MusicView;
}

class MusicView : public QWidget
{
    Q_OBJECT

public:
    explicit MusicView(QWidget *parent = 0);
    ~MusicView();

    void showSongs(const QList<Song> &songs);


private:
    Ui::MusicView *ui;
    SongView *songView;
};

#endif // MUSICVIEW_H
