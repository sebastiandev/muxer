QT  += xml sql

INCLUDEPATH += ../

HEADERS += \
    database/tagnormalizator.h \
    database/songindexer.h \
    database/songfinder.h \
    database/genrexmlreader.h \
    database/genresteammer.h \
    database/similaritydataservice.h \
    database/IndexerException.h

SOURCES += \
    database/songindexer.cpp \
    database/songfinder.cpp \
    database/tagnormalizator.cpp \
    database/genrexmlreader.cpp \
    database/genresteammer.cpp \
    database/similaritydataservice.cpp







