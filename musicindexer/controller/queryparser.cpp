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
    TagNormalizator normalizator(ConfigurationManager::GetString("genresdb"), 2);

    queryList = query.split(" ");
    for(int i=0; i<queryList.size(); i++)
    {
        QString qterm = queryList.at(i);
        expandedQuery << qterm;

        QString normedQ = normalizator.normalizeGenre(qterm);
        if (qterm != normedQ && GenreManager::manager().hasGenre(normedQ))
        {
            expandedQuery << normedQ;//add the normalized genre
        }

        // if there are 1 or more words left, those can make up a genre with the current term (ej. rock and roll, where rock is the current)
        if (queryList.size() - (i+1) >= 1)// (i+1) because index starts in 0
        {
            qterm += " " + queryList.at(i+1);
            QString aux = normalizator.normalizeGenre(qterm);

            // we try building a two word genre
            if (GenreManager::manager().hasGenre(aux))
            {
                expandedQuery << aux;//add the normalized genre
            }

            //try building a 3 word genre
            if (queryList.size() - (i+1) >= 2)
            {
                qterm += " " + queryList.at(i+2);
                QString aux = normalizator.normalizeGenre(qterm);

                if (GenreManager::manager().hasGenre(aux))
                {
                    expandedQuery << aux;//add the normalized genre
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
