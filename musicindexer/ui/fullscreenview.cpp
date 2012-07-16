#include "fullscreenview.h"
#include "ui_fullscreenview.h"
#include <QDesktopWidget>
FullScreenView::FullScreenView(QWidget *parent) :
    QWidget(parent, Qt::CustomizeWindowHint/*, Qt::FramelessWindowHint*/),
    ui(new Ui::FullScreenView)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_TranslucentBackground,true);

    //showFullScreen();
    setGeometry( QApplication::desktop()->availableGeometry());
    ui->wrapper->setGeometry(rect());

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(10);
    effect->setXOffset(0.0);
    effect->setYOffset(1.0);
    ui->searchBox->setGraphicsEffect(effect);

    _importView.reset(new ImportView(this));
    _importView->hide();

    _musicView.reset(new MusicView(this));
    _musicView->hide();

    connect(ui->btnAddMusic   , SIGNAL(clicked()), this, SLOT(slotImportMusic()));
    connect(ui->btnCollection , SIGNAL(clicked()), this, SIGNAL(showCollectionRequest()));
    connect(ui->btnQuit       , SIGNAL(clicked()), this, SIGNAL(quit()));
    connect(ui->btnQuit       , SIGNAL(clicked()), this, SLOT(close()));

    connect(_importView.data(), SIGNAL(importFolderSelected(QString&)), this, SIGNAL(importFolderRequest(QString&)));
}

FullScreenView::~FullScreenView()
{
    delete ui;
}

void FullScreenView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void FullScreenView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - _dragPosition);
        event->accept();
    }
}

bool FullScreenView::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
        setCursor(Qt::ArrowCursor);
    else if(e->type() == QEvent::MouseButtonPress)
        setCursor(Qt::ClosedHandCursor);

    return false;
}

void FullScreenView::slotImportMusic()
{
    QLayoutItem* item = ui->body->layout()->takeAt(0);
    if (item && item->widget())
        item->widget()->hide();

    ui->body->layout()->addWidget(_importView.data());
    _importView->show();
}

void FullScreenView::slotShowMusic(const QStringList &songs)
{
    QLayoutItem* item = ui->body->layout()->takeAt(0);
    if (item && item->widget())
        item->widget()->hide();

    ui->body->layout()->addWidget(_musicView.data());
    _musicView->show();
}

void FullScreenView::slotShowProgress()
{
    if (_importView->isVisible())
        _importView->showProgress();
}

void FullScreenView::slotUpdateProgress(int val)
{
    if (_importView->isVisible())
        _importView->updateProgress(val);
}

void FullScreenView::slotHideProgress()
{
    if (_importView->isVisible())
        _importView->hideProgress();
}

