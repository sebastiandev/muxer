#ifndef SONGINDEXER_H
#define SONGINDEXER_H

#include <QMap>
#include <QStringList>
#include "xapian.h"
#include "entities/song.h"

class SongIndexer
{
public:

    SongIndexer(){}
    SongIndexer(Xapian::WritableDatabase &database, QStringList stopWords=QStringList());

    void setDataBase(Xapian::WritableDatabase &database);
    void index      (const QString &path, const Song &song);

private:

    Xapian::WritableDatabase db;
    QStringList _stopwords;

    void addTermsToDocument           (Xapian::Document &doc, const QString &buffer, const QString &separator=QString(" "));
    void addNormalizedGenresToDocument(Xapian::Document &doc, const QStringList &buffer);

};

#endif // SONGINDEXER_H
