#include <QtTest/QtTest>
#include <QDebug>
#include <iostream>

#include "musicmanager.h"
#include "IConfigurator.h"
#include "ConfigurationManager.h"
#include "IniConfigurator.h"
#include "similaritydataservice.h"
#include "entities/album.h"

class TestSimilarityDataService: public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void testCreateTables();
    void testAddAlbum();
    void testRetrieveAlbumInfo();
    void testAlbumsBy();

private:
    IConfigurator *configurator;

};

void TestSimilarityDataService::initTestCase()
{
    qDebug() << "Loading configuration from resources/config.conf...";

    configurator = (IConfigurator *) new IniConfigurator("../resources/config.conf");
    ConfigurationManager::SetConfigurator(configurator);
}

void TestSimilarityDataService::testCreateTables()
{
    try
    {
        SimilarityDataService::service();
    }
    catch (...)
    {
        QFAIL("Excepcion!");
        std::cerr << "Excepcion! " << std::endl;
    }
}

void TestSimilarityDataService::testAddAlbum()
{
    try
    {
        // 1st album
        Album album("Peace sign", "Richie Kotzen");

        album.addSong(Song("best of times", "peace sign", "Richie Kotzen", "2009", "Rock, Soul", "", 100, "guitar"));
        album.addSong(Song("my messiah"   , "peace sign", "Richie Kotzen", "2009", "Rock, Soul", "", 110, "guitar"));
        album.addSong(Song("peace sign"   , "peace sign", "Richie Kotzen", "2009", "Rock, Soul", "", 120, "guitar"));
        album.addSong(Song("entretainer"  , "peace sign", "Richie Kotzen", "2009", "Rock, Soul", "", 80, "guitar"));

        QList<QPair<QString, int> > genres, types;

        genres.append(QPair<QString, int> ("Rock", 3));
        genres.append(QPair<QString, int> ("Soul", 1));

        types.append(QPair<QString, int> ("Guitar", 2));
        types.append(QPair<QString, int> ("Voice", 2));

        SimilarityDataService::service().addAlbumData(album, genres, types);

        // 2nd album
        Album album2("Go Faster", "Richie Kotzen");

        album2.addSong(Song("go faster", "Go Faster", "Richie Kotzen", "2007", "Rock, Hard Rock", "", 110, "guitar"));
        album2.addSong(Song("faith"   , "Go Faster", "Richie Kotzen", "2007", "Rock, Soul", "", 80, "guitar"));
        album2.addSong(Song("fooled again"   , "Go Faster", "Richie Kotzen", "2007", "Rock, Hard Rock", "", 120, "guitar"));
        album2.addSong(Song("bad things"  , "Go Faster", "Richie Kotzen", "2007", "Rock", "", 90, "guitar"));

        genres.clear(); types.clear();

        genres.append(QPair<QString, int> ("Rock", 4));
        genres.append(QPair<QString, int> ("Hard Rock", 2));
        genres.append(QPair<QString, int> ("Soul", 1));

        types.append(QPair<QString, int> ("Guitar", 3));
        types.append(QPair<QString, int> ("Voice", 1));

        SimilarityDataService::service().addAlbumData(album2, genres, types);

        // 3rd album
        Album album3("Slow", "Richie Kotzen");

        album3.addSong(Song("slow", "Slow", "Richie Kotzen", "2002", "Rock, Hard Rock", "", 110, "guitar"));
        album3.addSong(Song("gold digger"   , "Slow", "Richie Kotzen", "2002", "Rock, Hard Rock", "", 80, "guitar"));
        album3.addSong(Song("The Answer"   , "Slow", "Richie Kotzen", "2002", "Fusion", "", 120, "guitar"));
        album3.addSong(Song("Scared of You"  , "Slow", "Richie Kotzen", "2002", "Hard Rock", "", 90, "guitar"));

        genres.clear(); types.clear();

        genres.append(QPair<QString, int> ("Rock", 2));
        genres.append(QPair<QString, int> ("Hard Rock", 3));
        genres.append(QPair<QString, int> ("Fusion", 1));

        types.append(QPair<QString, int> ("Guitar", 4));

        SimilarityDataService::service().addAlbumData(album3, genres, types);

    }
    catch (...)
    {
        QFAIL("Excepcion!");
        std::cerr << "Excepcion! " << std::endl;
    }
}

void TestSimilarityDataService::testRetrieveAlbumInfo()
{
    try
    {
        Album album("Slow", "Richie Kotzen");
        AlbumDataObject dataObject = SimilarityDataService::service().getAlbumData(album);

        QVERIFY2(!dataObject.getArtist().isEmpty(), "Tiene que haber encontrado el registro de Richie Kotzen - Slow");

        QString info =  dataObject.getArtist() + " - " + dataObject.getTitle() + "[ Average Tempo: " + QString::number(dataObject.getAverageTempo()) + ", ";

        for(int i=0; i<dataObject.getGenreOcurrencies().size(); i++)
            info += dataObject.getGenreOcurrencies().at(i).first + ": " + QString::number((int)dataObject.getGenreOcurrencies().at(i).second) + ", ";

        for(int i=0; i<dataObject.getTypeOcurrencies().size(); i++)
            info += dataObject.getTypeOcurrencies().at(i).first + ": " + QString::number((int)dataObject.getTypeOcurrencies().at(i).second) + ", ";

        info += "]";

        qDebug() << info;

     }
    catch (...)
    {
        QFAIL("Excepcion!");
        std::cerr << "Excepcion! " << std::endl;
    }
}

void TestSimilarityDataService::testAlbumsBy()
{
    try
    {
        Album album("Slow", "Richie Kotzen");

        album.addSong(Song("slow", "Slow", "Richie Kotzen", "2002", "Rock, Hard Rock", "", 110, "guitar"));
        album.addSong(Song("gold digger"   , "Slow", "Richie Kotzen", "2002", "Rock, Hard Rock", "", 80, "guitar"));
        album.addSong(Song("The Answer"   , "Slow", "Richie Kotzen", "2002", "Fusion", "", 120, "guitar"));
        album.addSong(Song("Scared of You"  , "Slow", "Richie Kotzen", "2002", "Hard Rock", "", 90, "guitar"));

        QStringList genres, types;

        genres.append("Hard Rock");
        genres.append("Rock");
        genres.append("Fusion");

        // types.append(QPair<QString, int> ("Guitar", 4));
        QPair<int, int> tempoRange;
        tempoRange.first  = album.getAverageTempo() - 15;
        tempoRange.second = album.getAverageTempo() + 15;

        QList<AlbumDataObject> result = SimilarityDataService::service().getAlbumsBy(album, tempoRange, genres, types);

        QVERIFY2(!result.isEmpty(), "Tiene que encontrar resultados!");

        foreach(AlbumDataObject obj, result)
        {
            QString info = "Album: " + obj.getTitle() + ", Artist: " + obj.getArtist() + "[ avg Tempo: " + QString::number(obj.getAverageTempo()) + ", ";

            for(int i=0; i<obj.getGenreOcurrencies().size(); i++)
            {
                info += "Genre: " + obj.getGenreOcurrencies().at(i).first + ", Ocurrencies: " + QString::number(obj.getGenreOcurrencies().at(i).second) + ",";
            }

            info += "]";

            qDebug() << info;
        }
    }
    catch (...)
    {
        QFAIL("Excepcion!");
        std::cerr << "Excepcion! " << std::endl;
    }

}


QTEST_MAIN(TestSimilarityDataService)
#include ".moc/testSimilarityDataService.moc"

