#include <QtTest/QtTest>
#include <QDebug>
#include <iostream>

#include "musicmanager.h"
#include "IConfigurator.h"
#include "ConfigurationManager.h"
#include "IniConfigurator.h"
#include "similaritydataservice.h"
#include "entities/album.h"
#include "controller/musicmanager.h"

class TestSimilarity: public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void testSimilarAlbums();
    void testSimilarArtists();

private:
    IConfigurator *configurator;

};

void TestSimilarity::initTestCase()
{
    qDebug() << "Loading configuration from resources/config.conf...";

    configurator = (IConfigurator *) new IniConfigurator("../resources/config.conf");
    ConfigurationManager::SetConfigurator(configurator);
}

void TestSimilarity::testSimilarAlbums()
{
    try
    {
        MusicManager::manager().addSongsFromDirectory("/media/Seba/Musica/Luis Salinas/");

        QList<Album> albums = MusicManager::manager().getAlbumsByArtist("Luis Salinas");

        //SimilarityManager similarityManager;
        QList<Album> similarAlbums = SimilarityManager::manager().getSimilarAlbums(albums.at(0));


        qDebug() << "Similar albums to: " << albums.at(0).getArtist() << " - " << albums.at(0).getTitle();
        foreach (const Album &album, similarAlbums)
            qDebug() << "  >> " << album.getTitle();

    }
    catch (...)
    {
        QFAIL("Excepcion!");
        std::cerr << "Excepcion! " << std::endl;
    }
}

void TestSimilarity::testSimilarArtists()
{
    try
    {
        Artist artist = MusicManager::manager().getArtist("Luis Salinas");

        //SimilarityManager similarityManager;
        QList<Artist> similarArtists = SimilarityManager::manager().getSimilarArtists(artist);


        qDebug() << "Similar artists to: " << artist.getName();
        foreach (const Artist &artist, similarArtists)
            qDebug() << "  >> " << artist.getName();

    }
    catch (...)
    {
        QFAIL("Excepcion!");
        std::cerr << "Excepcion! " << std::endl;
    }

}


QTEST_MAIN(TestSimilarity)
#include ".moc/testSimilarity.moc"
