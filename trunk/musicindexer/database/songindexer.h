#ifndef SONGINDEXER_H
#define SONGINDEXER_H

#include <QMap>
#include "xapian.h"
#include "entities/song.h"

class SongIndexer
{
public:

    SongIndexer(){}
    SongIndexer(Xapian::WritableDatabase &database);

    void setDataBase(Xapian::WritableDatabase &database);
    void index      (const QString &path, const Song &song);

private:

    Xapian::WritableDatabase db;

    void addTermsToDocument           (Xapian::Document &doc, const QString &buffer, const QString &separator=QString(" "));
    void addNormalizedGenresToDocument(Xapian::Document &doc, const QStringList &buffer);

};

#endif // SONGINDEXER_H
