#ifndef GENRE_H
#define GENRE_H

#include <QStringList>

class Genre
{
public:
    Genre();
    Genre(QString id, QString display, QList<Genre> variants, QList<Genre> subGenres){ _id = id; _display = display; _variants = variants; _subGenres = subGenres;}
    Genre(QString id, QString display) { _id = id; _display = display;}
    Genre(QString id) { _id = id;}

    QString      getId       () const { return _id;}
    QString      getDisplay  () const { return _display;}
    QList<Genre> getVariants () const { return _variants;}
    QList<Genre> getSubGenres() const { return _subGenres;}

    void         setId       (const QString &val)     { _id = val;}
    void         setDisplay  (const QString &val)     { _display = val;}
    void         setVariants (const QList<Genre> &val){ _variants = val;}
    void         setSubGenres(const QList<Genre> &val){ _subGenres = val;}

    void         addSubGenre (const Genre &subgenre)  { _subGenres.append(subgenre);}
    void         addVariant  (const Genre &variant)   { _variants.append(variant);}

    bool         hasSubGenre (const QString &id) const { return _subGenres.contains(Genre(id));}
    bool         hasVariant  (const QString &id) const { return _variants.contains(Genre(id));}

    Genre        getSubGenre (const QString &id) const { return _subGenres.value( _subGenres.indexOf(Genre(id)) );}
    Genre        getVariant  (const QString &id) const { return _variants.value( _variants.indexOf(Genre(id)) );}


    friend bool operator ==(const Genre &g1, const Genre &g2){ return g1.getId() == g2.getId();}
    friend bool operator < (const Genre &g1, const Genre &g2){ return g1.getId() <  g2.getId();}

private:

    QString      _id, _display;
    QList<Genre> _variants, _subGenres;
};

#endif // GENRE_H
