QT       += core xml testlib sql gui


TARGET   = test
TEMPLATE = app

CONFIG   += order
CONFIG   -= app_bundle

OBJECTS_DIR = .obj/
MOC_DIR     = .moc/

INCLUDEPATH += . ../../musicindexer/ ../../musicindexer/controller ../../musicindexer/entities ../../musicindexer/database ../../musicindexer/configuration ../../musicindexer/logging ../../musicindexer/util

LIBS += -L'../../musicindexer/.obj/' \
        ../../musicindexer/.obj/genre.o \
        ../../musicindexer/.obj/genresteammer.o \
        ../../musicindexer/.obj/distancecalculator.o \
        ../../musicindexer/.obj/genrexmlreader.o \
        ../../musicindexer/.obj/QFileUtils.o \
        ../../musicindexer/.obj/tagnormalizator.o \
        ../../musicindexer/.obj/genremanager.o \
        ../../musicindexer/.obj/songindexer.o \
        ../../musicindexer/.obj/songfinder.o \
        ../../musicindexer/.obj/entitiesutil.o \
        ../../musicindexer/.obj/IniConfigurator.o \
        ../../musicindexer/.obj/ConfigurationManager.o \
        ../../musicindexer/.obj/LoggerManager.o \
        ../../musicindexer/.obj/FileLogger.o \
        ../../musicindexer/.obj/queryparser.o \
        ../../musicindexer/.obj/similaritymanager.o \
        ../../musicindexer/.obj/similaritydataservice.o \
        ../../musicindexer/.obj/musicmanager.o \
        -lxapian /usr/lib/libtag.so

SOURCES += \
    testSimilarity.cpp






