#include "compactplaylist.h"
#include "ui_compactplaylist.h"

CompactPlaylist::CompactPlaylist(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
    ui(new Ui::CompactPlaylist)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);

    _currentSong = 0;

    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->resizeSection(4, 20);


    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(slotSongSelected(int, int)));
}

CompactPlaylist::~CompactPlaylist()
{
    delete ui;
}

void CompactPlaylist::addSong(const Song &song, const QString &file)
{
    _playlist.insert(song.getArtist()+song.getAlbum()+song.getTitle(), QPair<Song, QString>(song, file));

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    int currentRow = ui->tableWidget->rowCount()-1;

    ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(song.getTitle().trimmed()));
    ui->tableWidget->item(currentRow, 0)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 1, new QTableWidgetItem(song.getArtist().trimmed()));
    ui->tableWidget->item(currentRow, 1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 2, new QTableWidgetItem(song.getAlbum().trimmed()));
    ui->tableWidget->item(currentRow, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 3, new QTableWidgetItem(QString::number(song.getLength()).trimmed()));
    ui->tableWidget->item(currentRow, 3)->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(currentRow, 4, new QTableWidgetItem(QIcon(":/icons/icons/remove.png"), ""));
}

void CompactPlaylist::slotSongSelected(int row, int col)
{
    // key = artist+album+song
    QString key = ui->tableWidget->item(row, 1)->text() + ui->tableWidget->item(row, 2)->text() + ui->tableWidget->item(row, 0)->text();

    if (col == 4)
    {
        _playlist.remove(key);
        ui->tableWidget->removeRow(row);
    }
    else
    {
        emit playSong(_playlist.value(key).first, _playlist.value(key).second);
        _currentSong = row;
    }
}

int CompactPlaylist::currentSong()
{
    return _currentSong+1;//because the index starts on 0
}

void CompactPlaylist::nextSong()
{
    if (_currentSong < _playlist.size()-1)//because the _currentSong index starts on 0
    {
        _currentSong++;
        QString key = ui->tableWidget->item(_currentSong, 1)->text() + ui->tableWidget->item(_currentSong, 2)->text() + ui->tableWidget->item(_currentSong, 0)->text();

        emit playSong(_playlist.value(key).first, _playlist.value(key).second);
    }
}

bool CompactPlaylist::contains(const Song &song)
{
    return _playlist.contains(song.getArtist()+song.getAlbum()+song.getTitle());
}

void CompactPlaylist::clear()
{
    _playlist.clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}
