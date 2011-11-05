QT       += core xml testlib sql

TARGET   = test
TEMPLATE = app

CONFIG   += order
CONFIG   -= app_bundle

OBJECTS_DIR = .obj/
MOC_DIR     = .moc/

INCLUDEPATH += . ../../musicindexer/ ../../musicindexer/controller ../../musicindexer/entities ../../musicindexer/database ../../musicindexer/configuration ../../musicindexer/logging ../../musicindexer/util

LIBS += -lxapian /usr/lib/libtag.so \
        -L'../../musicindexer/.obj/' \
        ../../musicindexer/.obj/musicmanager.o \
        ../../musicindexer/.obj/genremanager.o \
        ../../musicindexer/.obj/genresteammer.o \
        ../../musicindexer/.obj/genre.o \
        ../../musicindexer/.obj/distancecalculator.o \
        ../../musicindexer/.obj/genrexmlreader.o \
        ../../musicindexer/.obj/songfinder.o \
        ../../musicindexer/.obj/songindexer.o \
        ../../musicindexer/.obj/tagnormalizator.o \
        ../../musicindexer/.obj/IniConfigurator.o \
        ../../musicindexer/.obj/ConfigurationManager.o \
        ../../musicindexer/.obj/LoggerManager.o \
        ../../musicindexer/.obj/FileLogger.o \
        ../../musicindexer/.obj/QFileUtils.o \
        ../../musicindexer/.obj/queryparser.o \
        ../../musicindexer/.obj/similaritymanager.o \
        ../../musicindexer/.obj/similaritydataservice.o \
        ../../musicindexer/.obj/entitiesutil.o

SOURCES += \        
        testIndexer.cpp



HEADERS += \
















