#ifndef GENREMANAGER_H
#define GENREMANAGER_H

#include <QString>
#include <QMap>
#include <QPair>

#include "entities/genre.h"
#include "database/genrexmlreader.h"

class GenreManager
{
public:

    ~GenreManager(){}

    static GenreManager& manager(const QString &genresFile = "../resources/genres.xml");

    QString toString();

    bool    hasGenre      (const QString &genreId);
    bool    hasMainGenre  (const QString &genreId);
    bool    isSubGenre    (const QString &genreId);

    Genre   getGenre      (const QString &genreId);
    Genre   getParentGenre(const QString &subgenreId);

    QPair<Genre, Genre> geSubGenreWithParent(const QString &subgenreId);

    QList<Genre> getAllGenres();

private:

    GenreManager();
    Genre findSubGenre(const QString &subgenreId);
    Genre findVariant (const QString &genreId);

    static GenreXMLReader       _genreReader;
    static QMap<QString, Genre> _genres;
};

#endif // GENREMANAGER_H
