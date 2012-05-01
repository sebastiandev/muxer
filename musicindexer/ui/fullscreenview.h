#ifndef FULLSCREENVIEW_H
#define FULLSCREENVIEW_H

#include <QWidget>
#include <QScopedPointer>
#include <QMouseEvent>
#include <QGraphicsBlurEffect>

#include "importview.h"
#include "musicview.h"

namespace Ui {
class FullScreenView;
}

class FullScreenView : public QWidget
{
    Q_OBJECT
    
public:
    explicit FullScreenView(QWidget *parent = 0);
    ~FullScreenView();
    
public Q_SLOTS:

    void importMusic();
    void showMusic  ();

Q_SIGNALS:

    void quit();

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    bool eventFilter    (QObject *obj, QEvent *e);

private:
    Ui::FullScreenView *ui;

    QPoint _dragPosition;
    QGraphicsBlurEffect *_blurEffect;
    QScopedPointer<ImportView> _importView;
    QScopedPointer<MusicView>  _musicView;
};

#endif // FULLSCREENVIEW_H
