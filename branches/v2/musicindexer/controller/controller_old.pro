#-------------------------------------------------
#
# Project created by QtCreator 2011-10-06T19:44:31
#
#-------------------------------------------------

QT       += core gui xml

TARGET = musicIndexer
CONFIG   += qtestlib
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lxapian

SOURCES += \

    ui/mainwindow.cpp \
    ui/main.cpp \
    genres/tagnormalizator.cpp \
    genres/genrexmlreader.cpp \
    genres/genresteammer.cpp \
    genres/genremanager.cpp \
    genres/genre.cpp \
    genres/distancecalculator.cpp \
    logging/QDebugLogger.cpp \
    logging/LoggerManager.cpp \
    logging/FileLogger.cpp \
    util/QFileUtils.cpp \
    controller/songindexer.cpp \
    controller/songfinder.cpp \
    controller/musicmanager.cpp \
    controller/mbaction.cpp \
    controller/controller.cpp \
    controller/actionsearch.cpp \
    controller/actionimport.cpp \
    configuration/IniConfigurator.cpp \
    configuration/ConfigurationManager.cpp

HEADERS += \
    ui/mainwindow.h \
    genres/tagnormalizator.h \
    genres/genrexmlreader.h \
    genres/genresteammer.h \
    genres/genremanager.h \
    genres/genre.h \
    genres/distancecalculator.h \
    logging/QDebugLogger.h \
    logging/LoggerManager.h \
    logging/ILogger.h \
    logging/FileLogger.h \
    util/worker.h \
    util/QFileUtils.h \
    util/exception.h \
    controller/songindexer.h \
    controller/songitem.h \
    controller/songfinder.h \
    controller/musicmanager.h \
    controller/mbaction.h \
    controller/controller.h \
    controller/actionsearch.h \
    controller/actionimport.h \
    configuration/IniConfigurator.h \
    configuration/IConfigurator.h \
    configuration/ConfigurationManager.h

FORMS += \
    ui/form.ui

RESOURCES += \
    ui/icons.qrc

OTHER_FILES += \
    genres/genres.xml \
    resources/config.conf





































