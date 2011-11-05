INCLUDEPATH += ../

HEADERS += \
    controller/musicmanager.h \
    controller/mbaction.h \
    controller/controller.h \
    controller/actionsearch.h \
    controller/actionimport.h \
    controller/genremanager.h \
    controller/queryparser.h \
    controller/similaritymanager.h

SOURCES += \
    controller/musicmanager.cpp \
    controller/mbaction.cpp \
    controller/controller.cpp \
    controller/actionsearch.cpp \
    controller/actionimport.cpp \
    controller/genremanager.cpp \
    controller/queryparser.cpp \
    controller/similaritymanager.cpp


LIBS += -lxapian /usr/lib/libtag.so










