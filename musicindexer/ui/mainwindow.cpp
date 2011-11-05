#include "mainwindow.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QListWidget>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent, Qt::WindowStaysOnTopHint)
{
    ui.setupUi(this);

    setWindowTitle("Muxer");
    installEventFilter(this);
    setAttribute(Qt::WA_TranslucentBackground,true);

    QRegion visibleArea(this->x() + 2, this->y() , this->width() - 4, this->height());
    setMask(visibleArea);

    move(QApplication::desktop()->availableGeometry().width()/2 - this->width()/2, 100);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setXOffset(-0.0);
    effect->setYOffset(-0.0);

    ui.widget->setGraphicsEffect(effect);

    _musicViewer = new MusicViewer(this);

    connect(this->ui.leInput     , SIGNAL(textChanged(QString)), this, SLOT(highlightInput()));
    connect(this->ui.btAdd       , SIGNAL(clicked()), this, SIGNAL(addResourcesTrigger()));
    connect(this->ui.btCollection, SIGNAL(clicked()), this, SIGNAL(getCollectionTrigger()));
    connect(this->ui.btSimilarity, SIGNAL(clicked()), this, SIGNAL(showSimilarityTrigger()));
    connect(this->ui.btSearch    , SIGNAL(clicked()), this, SLOT(searchClicked()));

}

void MainWindow::highlightInput()
{

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
    emit searchTrigger(ui.leInput->text());
}

void MainWindow::slotAskForDirectory(QString *dir)
{
    *dir = QFileDialog::getExistingDirectory(0, "Import folder", QDir::homePath());
}

void MainWindow::slotShowSearchResult(const QStringList &result)
{
    QListWidget *list = new QListWidget();

    list->setGeometry(this->x() - ((400 - this->width())/2), this->y() + this->height() + 50, 400, 400);

    foreach (const QString &item, result)
        list->addItem(new QListWidgetItem(item));

    list->show();
}

void MainWindow::slotShowSongs(const QStringList &songList)
{
    _musicViewer->showSongs(songList);
}

void MainWindow::slotShowSimilarity(const QStringList &songlist)
{
    _musicViewer->showSimilarityScreen(songlist);
}
