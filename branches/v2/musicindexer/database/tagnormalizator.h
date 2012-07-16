#ifndef TAGNORMALIZATOR_H
#define TAGNORMALIZATOR_H

#include <QString>
#include <QMap>

class TagNormalizator
{
public:
    TagNormalizator(){}
    TagNormalizator(const QString &genresdb, int distanceThreshold = 1);
    TagNormalizator(int distanceThreshold = 1);

    QMap<QString, QString> normalizeTags(const QMap<QString, QString> &tags);

    QStringList normalizeGenre    (const QString &genre);
    QString     normalizeArtist   (const QString &artist);
    QString     normalizeAlbum    (const QString &album);
    QString     normalizeAlbumDate(const QString &albumDate);

private:

    int _distanceSimilarityThreshold;
};

#endif // TAGNORMALIZATOR_H
