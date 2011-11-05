#include "genremanager.h"
#include <iostream>

QMap<QString, Genre> GenreManager::_genres;
GenreXMLReader       GenreManager::_genreReader;

GenreManager::GenreManager(){}

GenreManager& GenreManager::manager(const QString &genresFile)
{
    static GenreManager instance;//allocate only the first time

    if (_genres.isEmpty())
        _genres = _genreReader.loadGenres(genresFile);


    return instance;
}

QString GenreManager::toString()
{
    QString info;

    QMapIterator<QString, Genre> it(_genres);
    while (it.hasNext())
    {
        it.next();
        Genre genre = it.value();

        info += "[" + genre.getDisplay() + " [variants ->";
        foreach(Genre var, genre.getVariants())
            info += var.getDisplay() + ",";

        info += "], [sub-genres ->";
        foreach(Genre sub, genre.getSubGenres())
            info += sub.getDisplay() + ",";

        info += "] \n";
    }

    info += "] \n";
    return info;
}

QList<Genre> GenreManager::getAllGenres()
{
    QList<Genre> list    = _genres.values();
    QList<Genre> listaux = list;

    foreach(const Genre &genre, listaux)
    {
        list.append(genre.getSubGenres());
        list.append(genre.getVariants());
    }

    return list;
}

Genre GenreManager::findVariant(const QString &genreId)
{
    Genre genre;
    QMapIterator<QString, Genre> it(_genres);
    while (it.hasNext())
    {
        it.next();
        Genre maingenre = it.value();

        if (maingenre.hasVariant(genreId))
        {
            genre = maingenre;//always return the maingenre
            break;
        }
    }

    return genre;
}

Genre GenreManager::findSubGenre(const QString &subgenreId)
{
    Genre subgenre;
    QMapIterator<QString, Genre> it(_genres);
    while (it.hasNext())
    {
        it.next();
        Genre maingenre = it.value();

        if (maingenre.hasSubGenre(subgenreId))
        {
            subgenre = maingenre.getSubGenre(subgenreId);
            break;
        }
    }

    return subgenre;
}

bool GenreManager::hasGenre(const QString &genreId)
{
    bool hasGenre = _genres.contains(genreId);
    if (!hasGenre)
    {
        hasGenre = findSubGenre(genreId).getId().isEmpty() ? false : true;

        if (!hasGenre)
            hasGenre = findVariant(genreId).getId().isEmpty() ? false : true;
    }

    return hasGenre;
}

bool GenreManager::hasMainGenre(const QString &genreId)
{
    return _genres.contains(genreId);
}

bool GenreManager::isSubGenre(const QString &genreId)
{
    return (findSubGenre(genreId).getId().isEmpty() ? false : true);
}

Genre GenreManager::getGenre(const QString &genreId)
{
    if (_genres.contains(genreId))
        return _genres.value(genreId);
    else
    {
        Genre genre = findSubGenre(genreId);
        if (genre.getId().isEmpty())
            genre = findVariant(genreId);

        return genre;
    }
}

Genre GenreManager::getParentGenre(const QString &subgenreId)
{
    Genre parentGenre;

    QMapIterator<QString, Genre> it(_genres);
    while (it.hasNext())
    {
        it.next();
        Genre genre = it.value();

        if (genre.hasSubGenre(subgenreId))
        {
            parentGenre = genre;
            break;
        }
    }

    return parentGenre;
}

QPair<Genre, Genre> GenreManager::geSubGenreWithParent(const QString &subgenreId)
{
    QPair<Genre, Genre> pair;
    pair.first  = getParentGenre(subgenreId);
    pair.second = getGenre(subgenreId);

    return pair;
}
