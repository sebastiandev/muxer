#include "tagnormalizator.h"
#include "genresteammer.h"
#include "controller/genremanager.h"
#include "util/distancecalculator.h"
#include "logging/LoggerManager.h"

#include <QVector>
#include <QtAlgorithms>

TagNormalizator::TagNormalizator(const QString &genresdb, int distanceThreshold)
{
    _distanceSimilarityThreshold = distanceThreshold;
    GenreManager::manager(genresdb);
}

TagNormalizator::TagNormalizator(int distanceThreshold)
{
    _distanceSimilarityThreshold = distanceThreshold;
    GenreManager::manager("genres.xml");
}


QMap<QString, QString> TagNormalizator::normalizeTags(const QMap<QString, QString> &tags)
{
    QMap<QString, QString> normalized;

    return normalized;
}

QStringList TagNormalizator::normalizeGenre(const QString &genre)
{
    if (genre.isEmpty()) return QStringList();

    Genre normalizedGenre;
    QStringList genres;

    QString steammedGenre = GenreSteammer::steam(genre);
    LoggerManager::LogDebug("[TagNormalizator] genre: " + genre + " --> " + steammedGenre);

    // look for the steammed genre in the genre db
    if (GenreManager::manager().hasGenre(steammedGenre))
    {
        normalizedGenre = GenreManager::manager().getGenre(steammedGenre);

        if (GenreManager::manager().isSubGenre(normalizedGenre.getId()))
        {
            genres << GenreManager::manager().getParentGenre(normalizedGenre.getId()).getId();
        }

        steammedGenre = normalizedGenre.getId();
        genres << steammedGenre;
    }
    else
    {
        // steammed genre is not in the db or it might be mispelled, look for the closests genres in the db
        QList<QPair<int, QString> > distances;
        Q_FOREACH(Genre genre, GenreManager::manager().getAllGenres())
        {
            int d = DistanceCalculator::distance(steammedGenre, genre.getId());
            distances.push_back( QPair<int, QString>(d, genre.getId()) );
        }

        //sort list and pic the first with the lower distance (if distance is lower than distanceThreshold)
        qSort(distances);
        int minDistance = distances.first().first;
        QString genreId = distances.first().second;

        if (minDistance <= _distanceSimilarityThreshold)
        {
            LoggerManager::LogDebug("[TagNormalizator] genre: '" + steammedGenre + "' not found in db.\nMin Distance: " +
                                    QString::number(minDistance) + ", to genre: " + genreId);

            // since we were comparing to every genre, we could have found a match with a variant of a genre
            // we must let the manager tell us the corresponding genre to that id
            steammedGenre = GenreManager::manager().getGenre(genreId).getId();
            genres << steammedGenre;
        }
        else
        {
            // exception, distances are larger than the threshold or its a new genre
            // leave the genre steammed without normalization
        }
    }

    return genres;
}

QString TagNormalizator::normalizeArtist(const QString &artist)
{
    QString normalized;

    return normalized;
}

QString TagNormalizator::normalizeAlbum(const QString &album)
{
    QString normalized;

    return normalized;
}

QString TagNormalizator::normalizeAlbumDate(const QString &albumDate)
{
    QString normalized;

    return normalized;
}
