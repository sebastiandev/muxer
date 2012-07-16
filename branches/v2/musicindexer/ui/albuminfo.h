#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include "ui_albuminfo.h"

class AlbumInfo : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumInfo(QWidget *parent = 0);

private:
    Ui::albumInfo ui;
};

#endif // ALBUMINFO_H
