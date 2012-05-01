#include "songindexer.h"
#include <QStringList>
#include "tagnormalizator.h"
#include "configuration/ConfigurationManager.h"
#include "logging/LoggerManager.h"
#include <iostream>
#include "IndexerException.h"

SongIndexer::SongIndexer(Xapian::WritableDatabase &database, QStringList stopWords)
{
    db = database;
    _stopwords = stopWords;
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

        if (!addTermsToDocument(doc, song.getTitle()))
        {
            IndexerException e("Empty song title", path);
            e.setErrorCode(IndexerException::EMPTY_TERM);
            throw (e);
        }

        if (!addTermsToDocument(doc, song.getArtist()))
        {
            IndexerException e("Empty song artist", path);
            e.setErrorCode(IndexerException::EMPTY_TERM);
            throw (e);
        }

        if (!addTermsToDocument(doc, song.getAlbum()))
        {
            IndexerException e("Empty song album", path);
            e.setErrorCode(IndexerException::EMPTY_TERM);
            throw (e);
        }

        if (!addTermsToDocument(doc, song.getYear()))
        {
            IndexerException e("Empty song year", path);
            e.setErrorCode(IndexerException::EMPTY_TERM);
            throw (e);
        }

        /*
        if (!addTermsToDocument(doc, song.getType(), ","))
        {
            IndexerException e("Empty song type", path);
            e.setErrorCode(IndexerException::EMPTY_TERM);
            throw (e);
        }
        */

        if (!addNormalizedGenresToDocument(doc, song.getGenres()))
        {
            IndexerException e("Empty song genre", path);
            e.setErrorCode(IndexerException::EMPTY_TERM);
            throw (e);
        }

        LoggerManager::LogDebug("[Song Indexer] --------------------------------------------------");

        db.add_document(doc);
        db.commit();
    }
    catch (const Xapian::Error &e)
    {
        std::cout << e.get_description() << std::endl;
        IndexerException ie(e.get_description().c_str(), path);
        ie.setErrorCode(IndexerException::DATABASE_ERROR);
        throw (ie);
    }
}

bool SongIndexer::addTermsToDocument(Xapian::Document &doc, const QString &terms, const QString &separator)
{
    // Indexes every single word from buffer, and then the whole thing joined with '_'
    if (terms.isEmpty())
    {
        return false;
    }
    else
    {
        QStringList termList = terms.split(separator);
        Q_FOREACH(const QString &term, termList)
        {
            if (!_stopwords.contains(term, Qt::CaseInsensitive))
            {
                LoggerManager::LogDebug("[Song Indexer] adding term " + term.trimmed().toLower());
                doc.add_term(term.trimmed().toLower().toStdString());
            }
        }

        // join only when there are 2 or more terms
        if (termList.size() > 1)
        {
            doc.add_term(termList.join("_").toLower().toStdString());
            LoggerManager::LogDebug("[Song Indexer] adding term " + termList.join("_").toLower());
        }
    }

    return true;
}

bool SongIndexer::addNormalizedGenresToDocument(Xapian::Document &doc, const QStringList &rawgenres)
{
    if (rawgenres.isEmpty())
    {
        return false;
    }
    else
    {
        TagNormalizator normalizator(ConfigurationManager::GetString("genresdb"), ConfigurationManager::GetInt("maxIndexerNormalizingDistance"));

        Q_FOREACH(const QString &rawgenre, rawgenres)
        {
            QStringList normalizedGenres = normalizator.normalizeGenre(rawgenre);
             Q_FOREACH(const QString& genre, normalizedGenres)
            {
                LoggerManager::LogDebug("[Song Indexer] adding normalized genre " + genre.trimmed().toLower());
                doc.add_term(genre.toStdString());
            }
        }
    }

    return true;
}

