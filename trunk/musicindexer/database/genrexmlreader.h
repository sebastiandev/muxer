#ifndef GENREXMLREADER_H
#define GENREXMLREADER_H

#include <QMap>
#include "entities/genre.h"

class GenreXMLReader
{
public:
    GenreXMLReader();

    QMap<QString, Genre> loadGenres(const QString &file);


private:


};

#endif // GENREXMLREADER_H
