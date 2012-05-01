#ifndef MUSICVIEW_H
#define MUSICVIEW_H

#include <QWidget>

namespace Ui {
class MusicView;
}

class MusicView : public QWidget
{
    Q_OBJECT
    
public:
    explicit MusicView(QWidget *parent = 0);
    ~MusicView();
    
    void addSong(const QString &artist,
                 const QString &album,
                 const QString &title,
                 const QString &duration,
                 const QString &genres,
                 const QString &quality);

private:
    Ui::MusicView *ui;
};

#endif // MUSICVIEW_H
