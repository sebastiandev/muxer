#include <QtTest/QtTest>
#include <QDebug>
#include <iostream>

#include "musicmanager.h"
#include "IConfigurator.h"
#include "ConfigurationManager.h"
#include "IniConfigurator.h"

class TestIndexer: public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void testIndexSongsFromHD();
    void testIndexSongAsMap();


private:
    IConfigurator *configurator;

};

void TestIndexer::initTestCase()
{
    qDebug() << "Loading configuration from resources/config.conf...";

    configurator = (IConfigurator *) new IniConfigurator("../resources/config.conf");
    ConfigurationManager::SetConfigurator(configurator);
}

void TestIndexer::testIndexSongAsMap()
{
    try
    {        
        QMap<QString, QString> song;
        song.insert("path"  , "home/usr/music/Richie Kotzen/Return of Mother Head Family Reunion/go faster.mp3");
        song.insert("artist", "Richie Kotzen");
        song.insert("title" , "Go Faster");
        song.insert("album" , "Return of Mother Head Family Reunion");
        song.insert("genre" , "rock, soul");
        song.insert("year"  , "2003");
        song.insert("type"  , "guitar");

        MusicManager::manager().addSongFromMap(song);

        song.clear();
        song.insert("path"  , "home/usr/music/The Rolling Stones/Bridges to Babylon/anybody.mp3");
        song.insert("artist", "The Rolling Stones");
        song.insert("title" , "Anybody seen my baby");
        song.insert("album" , "Bridge To Babylon");
        song.insert("genre" , "rock and roll, rock");
        song.insert("year"  , "1996");
        song.insert("type"  , "band");

        MusicManager::manager().addSongFromMap(song);

        QString query = "rock_roll Kotzen";
        QStringList results = MusicManager::manager().search(query);

        QVERIFY2(!results.isEmpty(), "Tiene que encontrar al menos un resultado!");

        foreach(const QString &r, results)
            std::cout << r.toStdString() << std::endl;               

    }
    catch (...)
    {
        std::cerr << "Excepcion! " << std::endl;
    }
}

void TestIndexer::testIndexSongsFromHD()
{
    try
    {
        MusicManager::manager().addSongsFromDirectory("/media/Seba/Musica/Richie Kotzen/Discografia/2009 - Peace Sign");

        QString query = "rock_roll Kotzen";
        QStringList results = MusicManager::manager().search(query);


        QVERIFY2(!results.isEmpty(), "Tiene que encontrar al menos un resultado!");

        foreach(const QString &r, results)
            std::cout << r.toStdString() << std::endl;

    }
    catch (...)
    {
        std::cerr << "Excepcion! " << std::endl;
    }

}



QTEST_MAIN(TestIndexer)
#include ".moc/testIndexer.moc"
