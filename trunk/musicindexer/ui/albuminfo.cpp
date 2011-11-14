#include "albuminfo.h"
#include <QGraphicsDropShadowEffect>

AlbumInfo::AlbumInfo(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setBlurRadius(20);

    ui.wWrapper->setGraphicsEffect(shadow);
}
