#ifndef SONGFINDER_H
#define SONGFINDER_H

#include <xapian.h>
#include <QStringList>

class SongFinder
{
public:
    SongFinder(){}
    SongFinder(Xapian::WritableDatabase &database);

    void        setDataBase(Xapian::WritableDatabase &database);
    QStringList search(const QStringList &strquery);
    QStringList searchExactTerms(const QStringList &terms);
    QStringList allSongs();

private:

    QStringList doSearch(const Xapian::Query& query);

    Xapian::WritableDatabase db;

};

#endif // SONGFINDER_H
