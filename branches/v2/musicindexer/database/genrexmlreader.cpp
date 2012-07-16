#include "genrexmlreader.h"
#include <QtXml>
#include <QFile>
#include <iostream>

GenreXMLReader::GenreXMLReader()
{
}

QMap<QString, Genre> GenreXMLReader::loadGenres(const QString &genrefile)
{
    QMap<QString, Genre> _genres;

    QDomDocument doc("genres");
    QFile        file(genrefile);
    QString errorMsg;
    int errorLine, errorColumn;

    if (!file.open(QIODevice::ReadOnly))
        return _genres;
    if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
    {
        file.close();
        std::cout << "Error: " << errorMsg.toStdString() << ".Linea: " << errorLine << ". Col: " << errorColumn << std::endl;
        return _genres;
    }

    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            Genre genre;
            genre.setId(e.tagName());
            genre.setDisplay(e.attribute("display"));

            QDomNodeList variantsList  = e.elementsByTagName("variants");
            QDomNodeList subGenresList = e.elementsByTagName("sub-genres");

            // load defined variants for the genre
            // should be only one element "variants"
            QDomNode variant = variantsList.at(0).firstChild();
            while (!variant.isNull() && variant.isElement())
            {
                genre.addVariant( Genre(variant.toElement().tagName(), variant.toElement().attribute("display")) );
                variant = variant.nextSibling();
            }

            // load sub-genres
            // should be only one element "sub-genres"
            QDomNode subGenre = subGenresList.at(0).firstChild();
            while (!subGenre.isNull() && subGenre.isElement())
            {
                genre.addSubGenre( Genre(subGenre.toElement().tagName(), subGenre.toElement().attribute("display")) );
                subGenre = subGenre.nextSibling();
            }

            _genres.insert(genre.getId(), genre);
        }

        n = n.nextSibling();
    }

    return _genres;
}
