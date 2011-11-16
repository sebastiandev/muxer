#include "queryparser.h"
#include "genremanager.h"
#include "database/tagnormalizator.h"
#include "configuration/ConfigurationManager.h"

QueryParser::QueryParser()
{
}

QueryParser& QueryParser::parser()
{
    static QueryParser instance;
    return instance;
}

QStringList QueryParser::expandQuery(const QString &query)
{
    // we cant use a big distance because it could replace terms from the query that werent mean to be genres
    QStringList expandedQuery, queryList;
    TagNormalizator normalizator(ConfigurationManager::GetString("genresdb"), ConfigurationManager::GetInt("maxQueryNormalizingDistance"));

    queryList = query.split(" ");
    for(int i=0; i<queryList.size(); i++)
    {
        QString qterm = queryList.at(i);
        expandedQuery << qterm;

        QStringList normedQ = normalizator.normalizeGenre(qterm);

        if (!normedQ.isEmpty())
        {
            QString normToLookFor = normedQ.size() > 1 ? normedQ.at(1) : normedQ.first();

            if (qterm != normToLookFor && GenreManager::manager().hasGenre(normToLookFor))
            {
                expandedQuery << normToLookFor;//add the normalized genre
            }
        }

        // if there are 1 or more words left, those can make up a genre with the current term (ej. rock and roll, where rock is the current)
        if (queryList.size() - (i+1) >= 1)// (i+1) because index starts in 0
        {
            qterm += " " + queryList.at(i+1);
            QStringList aux = normalizator.normalizeGenre(qterm);

            if (!aux.isEmpty())
            {
                QString normToLookFor = aux.size() > 1 ? aux.at(1) : aux.first();

                // we try building a two word genre
                if (GenreManager::manager().hasGenre(normToLookFor))
                {
                    expandedQuery << normToLookFor;//add the normalized genre
                }
            }

            //try building a 3 word genre
            if (queryList.size() - (i+1) >= 2)
            {
                qterm += " " + queryList.at(i+2);
                QStringList aux = normalizator.normalizeGenre(qterm);

                if (!aux.isEmpty())
                {
                    QString normToLookFor = aux.size() > 1 ? aux.at(1) : aux.first();

                    if (GenreManager::manager().hasGenre(normToLookFor))
                    {
                        expandedQuery << normToLookFor;//add the normalized genre
                    }
                }
            }
        }

    }

    return expandedQuery;
}

bool QueryParser::queryHasGenres(const QStringList &query)
{

}

QList<Genre> QueryParser::getGenresFromQuery(const QStringList &query)
{

}
