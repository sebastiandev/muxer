#include "songindexer.h"
#include <QStringList>
#include "tagnormalizator.h"
#include "configuration/ConfigurationManager.h"
#include "logging/LoggerManager.h"
#include <iostream>

SongIndexer::SongIndexer(Xapian::WritableDatabase &database)
{
    db = database;
}

void SongIndexer::setDataBase(Xapian::WritableDatabase &database)
{
    db = database;
}

void SongIndexer::index(const QString &path, const Song &song)
{
    try
    {
        Xapian::Document doc;
        doc.set_data(path.toStdString());

        LoggerManager::LogDebug("[Song Indexer] indexing " + path);
        LoggerManager::LogDebug("[Song Indexer] --------------------------------------------------");

        addTermsToDocument(doc, song.getTitle());
        addTermsToDocument(doc, song.getArtist());
        addTermsToDocument(doc, song.getAlbum());
        addTermsToDocument(doc, song.getYear());
        addTermsToDocument(doc, song.getType(), ",");

        addNormalizedGenresToDocument(doc, song.getGenres());

        LoggerManager::LogDebug("[Song Indexer] --------------------------------------------------");

        db.add_document(doc);
        db.commit();
    }
    catch (const Xapian::Error &e)
    {
        std::cout << e.get_description() << std::endl;
    }
}

void SongIndexer::addTermsToDocument(Xapian::Document &doc, const QString &buffer, const QString &separator)
{
    // Indexes every single word from buffer, and then the whole thing joined with '_'
    if (!buffer.isEmpty())
    {
        QStringList var = buffer.split(separator);
        foreach(const QString &str, var)
        {
            LoggerManager::LogDebug("[Song Indexer] adding term " + str.trimmed().toLower());
            doc.add_term(str.trimmed().toLower().toStdString());
        }

        // join only when there are 2 or more terms
        if (var.size() > 1)
        {
            doc.add_term(var.join("_").toLower().toStdString());
            LoggerManager::LogDebug("[Song Indexer] adding term " + var.join("_").toLower());
        }
    }
}

void SongIndexer::addNormalizedGenresToDocument(Xapian::Document &doc, const QStringList &buffer)
{
    if (!buffer.isEmpty())
    {
        TagNormalizator normalizator(ConfigurationManager::GetString("genresdb"), 4);

        foreach(const QString &str, buffer)
        {
            QString normalizedTermn = normalizator.normalizeGenre(str).trimmed().toLower();
            LoggerManager::LogDebug("[Song Indexer] adding genre " + normalizedTermn);
            doc.add_term(normalizedTermn.toStdString());
        }
    }
}

