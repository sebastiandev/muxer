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


private:
    IConfigurator *configurator;

};

void TestIndexer::initTestCase()
{
    qDebug() << "Loading configuration from resources/config.conf...";

    configurator = (IConfigurator *) new IniConfigurator("../resources/config.conf");
    ConfigurationManager::SetConfigurator(configurator);
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
