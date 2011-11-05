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

QString TagNormalizator::normalizeGenre(const QString &genre)
{
    if (genre.isEmpty()) return "";

    QString normalized = GenreSteammer::steam(genre);
    LoggerManager::LogDebug("[TagNormalizator] genre: " + genre + " --> " + normalized);

    if (GenreManager::manager().hasGenre(normalized))
    {
        normalized = GenreManager::manager().getGenre(normalized).getId();
    }
    else
    {
        QList<QPair<int, QString> > distances;
        foreach (Genre genre, GenreManager::manager().getAllGenres())
        {
            int d = DistanceCalculator::distance(normalized, genre.getId());
            distances.push_back( QPair<int, QString>(d, genre.getId()) );
        }

        //sort list and pic the first with the lower distance (if distance is lower than distanceThreshold)
        qSort(distances);
        int minDistance = distances.first().first;
        QString genreId = distances.first().second;

        if (minDistance <= _distanceSimilarityThreshold)
        {
            // since we were comparing to every genre, we could have foud a match with a variant of a genre
            // we must let the manager tell us the corresponding genre to that id
            normalized = GenreManager::manager().getGenre(genreId).getId();
            LoggerManager::LogDebug("[TagNormalizator] genre: '" + genre + "' not found in db.\nMin Distance: " +
                                    QString::number(minDistance) + ", to genre: " + genreId);

        }
        else
        {
            // exception, distances are larger than the threshold!
            // leave the genre steammed without normalization
        }
    }

    return normalized;
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
