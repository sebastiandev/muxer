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
    _musicView.reset(new MusicView(this));
    _importView->hide();
    _musicView->hide();

    connect(ui->btnAddMusic, SIGNAL(clicked()), this, SLOT(importMusic()));
    connect(ui->btnCollection, SIGNAL(clicked()), this, SLOT(showMusic()));
    connect(ui->btnQuit, SIGNAL(clicked()), this, SLOT(close()));
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

void FullScreenView::importMusic()
{
    QLayoutItem* item = ui->body->layout()->takeAt(0);
    if (item && item->widget())
        item->widget()->hide();

    ui->body->layout()->addWidget(_importView.data());
    _importView->show();
}

void FullScreenView::showMusic()
{
    QLayoutItem* item = ui->body->layout()->takeAt(0);
    if (item && item->widget())
        item->widget()->hide();

    ui->body->layout()->addWidget(_musicView.data());
    _musicView->show();
}
