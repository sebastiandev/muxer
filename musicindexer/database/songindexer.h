#ifndef SONGINDEXER_H
#define SONGINDEXER_H

#include <QMap>
#include <QStringList>
#include "xapian.h"
#include "xapian/database.h"

#include "entities/song.h"

class SongIndexer
{
public:

    SongIndexer(){}
    SongIndexer(Xapian::WritableDatabase &database, QStringList stopWords=QStringList());

    void setDataBase (Xapian::WritableDatabase &database);
    void setStopWords(const QStringList &stopwords){ _stopwords = stopwords;}

    void index       (const QString &path, const Song &song);

private:

    Xapian::WritableDatabase db;
    QStringList _stopwords;

    bool addTermsToDocument           (Xapian::Document &doc, const QString &buffer, const QString &separator=QString(" "));
    bool addNormalizedGenresToDocument(Xapian::Document &doc, const QStringList &buffer);

};

#endif // SONGINDEXER_H
