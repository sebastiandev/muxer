#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <QStringList>
#include "entities/genre.h"

class QueryParser
{
public:

    ~QueryParser(){}

    static QueryParser& parser();

    QStringList  expandQuery(const QString &query);
    bool         queryHasGenres(const QStringList &query);
    QList<Genre> getGenresFromQuery(const QStringList &query);


private:
    QueryParser();
};

#endif // QUERYPARSER_H
