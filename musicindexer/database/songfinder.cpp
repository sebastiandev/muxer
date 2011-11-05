#include "songfinder.h"
#include <QDebug>
#include "tagnormalizator.h"
#include "configuration/ConfigurationManager.h"

SongFinder::SongFinder(Xapian::WritableDatabase &database)
{
    db = database;
}

void SongFinder::setDataBase(Xapian::WritableDatabase &database)
{
    db = database;
}

QStringList SongFinder::allSongs()
{
    QStringList songs;

    Xapian::Database qdb = Xapian::Database(db);
    qDebug() << "There are: " << qdb.get_doccount()  << " songs";
    qDebug() << "There are: " << qdb.get_lastdocid() << " songs";

    for (int i=1; i<=qdb.get_doccount(); i++)
    {
        songs << QString(qdb.get_document((Xapian::docid)i).get_data().c_str());
    }

    qDebug() << "Stringlist size: " << songs.size();

    return songs;
}

QStringList SongFinder::search(const QStringList &strquery)
{
    Xapian::Query query(strquery.first().toLower().toStdString());

    for (int i=1; i <strquery.size(); i++)
        query = Xapian::Query(Xapian::Query::OP_OR, query, Xapian::Query(strquery.at(i).toLower().toStdString()));

    return doSearch(query);
}

QStringList SongFinder::searchExactTerms(const QStringList &strquery)
{
    Xapian::Query query(strquery.first().toLower().toStdString());

    for (int i=1; i <strquery.size(); i++)
        query = Xapian::Query(Xapian::Query::OP_AND, query, Xapian::Query(strquery.at(i).toLower().toStdString()));

    return doSearch(query);
}

QStringList SongFinder::doSearch(const Xapian::Query &query)
{
    qDebug() << "query: " << QString(query.get_description().c_str());

    Xapian::Enquire enquire(db);
    enquire.set_query(query);

    QStringList result;
    Xapian::MSet matches = enquire.get_mset(0, 1000000);
    for(Xapian::MSetIterator it = matches.begin(); it != matches.end(); it++)
    {
        result << QString(it.get_document().get_data().c_str());
        //qDebug() << QString(it.get_document().get_data().c_str());
        //for(Xapian::TermIterator it2 = it.get_document().termlist_begin(); it2 != it.get_document().termlist_end(); it2++)
        //    qDebug() << QString(std::string(*it2).c_str());
    }

    result.sort();
    return result;
}
