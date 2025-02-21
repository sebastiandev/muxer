#include "mainwindow.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QListWidget>
#include <QDesktopWidget>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    ui.setupUi(this);

    setWindowTitle("Muxer");
    installEventFilter(this);
    setAttribute(Qt::WA_TranslucentBackground,true);

    /*
    QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height());
    setMask(visibleArea);
    */
    move(QApplication::desktop()->availableGeometry().width()/2 - this->width()/2, 100);

    ui.widget_3->hide();
    resize(730, 50);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setXOffset(-0.0);
    effect->setYOffset(-0.0);

    ui.wContainer->setGraphicsEffect(effect);

    _musicViewer = new MusicViewer(this);
    _player = NULL;

    connect(this->ui.leInput     , SIGNAL(textChanged(QString)), this, SLOT(highlightInput()));
    connect(this->ui.btAdd       , SIGNAL(clicked()), this, SIGNAL(addResourcesTrigger()));
    connect(this->ui.btCollection, SIGNAL(clicked()), this, SIGNAL(getCollectionTrigger()));
    connect(this->ui.btSimilarity, SIGNAL(clicked()), this, SIGNAL(showSimilarityTrigger()));
    connect(this->ui.btSearch    , SIGNAL(clicked()), this, SLOT(searchClicked()));
    connect(this->ui.btClose     , SIGNAL(clicked()), this, SLOT(slotClose()));
    connect(_musicViewer, SIGNAL(playSong(const Song&, const QString&)), this, SLOT(slotPlaySongRequested(const Song&, const QString&)));
}

void MainWindow::highlightInput()
{

}

void MainWindow::slotShowProgress()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(300);
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height() + 20));
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->start();

    ui.widget_3->show();
}

void MainWindow::slotUpdateProgress(int progress)
{
    ui.progressBar->setValue(progress);
    ui.progressBar->repaint();
}

void MainWindow::slotHideProgress()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(300);
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height() - 24));
    animation->setEasingCurve(QEasingCurve::OutQuart);
    animation->start();

    ui.widget_3->hide();
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        if (_player)
            _player->updatePosition(event->globalPos() - m_dragPosition);

        event->accept();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
        setCursor(Qt::ArrowCursor);
    else if(e->type() == QEvent::MouseButtonPress)
        setCursor(Qt::ClosedHandCursor);

    return false;
}

void MainWindow::searchClicked()
{
    Q_EMIT searchTrigger(ui.leInput->text());
}

void MainWindow::slotAskForDirectory(QString *dir)
{
    *dir = QFileDialog::getExistingDirectory(0, "Import folder", QDir::homePath());
}

void MainWindow::slotShowSongs(const QStringList &songList)
{
    _musicViewer->showSongs(songList);
}

void MainWindow::slotShowSimilarity(const QStringList &songlist)
{
    _musicViewer->showSimilarityScreen(songlist);
}

void MainWindow::slotPlaySongRequested(const Song &song, const QString &songFile)
{
    if (_player == NULL)
    {
        _player = new CompactPlayer();
        ui.wExpandMenu->layout()->addWidget(_player);

        _player->show();
        QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
        animation->setDuration(300);
        animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
        animation->setEndValue(QRect(this->x()-200, this->y(), this->width() + 200, this->height()));
        animation->setEasingCurve(QEasingCurve::OutQuart);
        animation->start();
    }

    if (_player->songsInQueue() == 0)
        _player->playSong(song, songFile);
    else
        _player->addSong(song, songFile);
}

void MainWindow::slotClose()
{
    _musicViewer->close();

    if (_player)
        _player->close();

    close();
}
