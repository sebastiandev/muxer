#include "musicview.h"
#include "ui_musicview.h"
#include <QGraphicsDropShadowEffect>

MusicView::MusicView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicView)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    effect->setXOffset(0.0);
    effect->setYOffset(4.0);

    ui->wToolBar->setGraphicsEffect(effect);

    ui->wInfo->hide();
    //ui->wView->resize(ui->wBody->size());
    //ui->tableWidget->resize(ui->wBody->size());

    //ui->tableWidget->hide();
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->resizeSection(0, 25);
    ui->tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setResizeMode(3, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(5, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableWidget->setRowCount(0);

    addSong("Richie Kotzen", "24 Hours", "OMG", "3:23", "Rock, Soul", "320");
    addSong("Richie Kotzen", "24 Hours", "Help Me", "4:03", "Rock, Soul", "320");
    addSong("Richie Kotzen", "24 Hours", "24 Hours", "5:30", "Rock, Soul", "320");
    addSong("Richie Kotzen", "24 Hours", "Bad Situation", "3:23", "Rock, Soul", "320");
}

MusicView::~MusicView()
{
    delete ui;
}

void MusicView::addSong(const QString &artist,
                        const QString &album,
                        const QString &title,
                        const QString &duration,
                        const QString &genres,
                        const QString &quality)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    int currentRow = ui->tableWidget->rowCount()-1;

    ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(QIcon(":icons/icons/play.png"), ""));

    ui->tableWidget->setItem(currentRow, 1, new QTableWidgetItem(title.trimmed()));
    ui->tableWidget->item(currentRow, 1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 2, new QTableWidgetItem(artist.trimmed()));
    ui->tableWidget->item(currentRow, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 3, new QTableWidgetItem(album.trimmed()));
    ui->tableWidget->item(currentRow, 3)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 4, new QTableWidgetItem(duration.trimmed()));
    ui->tableWidget->item(currentRow, 4)->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 5, new QTableWidgetItem(genres.trimmed()));
    ui->tableWidget->item(currentRow, 5)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 6, new QTableWidgetItem(quality.trimmed()));
    ui->tableWidget->item(currentRow, 6)->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
}
