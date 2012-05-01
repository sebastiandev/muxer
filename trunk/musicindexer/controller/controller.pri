INCLUDEPATH += ../

win32 {
    INCLUDEPATH += ../thirdParty/taglib-1.7.1/include

    CONFIG(release, debug|release) {
      message( "buidling in release" )
      BUILD = release
    }
    CONFIG(debug, debug|release) {
      message( "building in debug" )
      BUILD = debug
    }

    LIBS += \
            kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
            advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib rpcrt4.lib\
            wsock32.lib Ws2_32.lib  odbccp32.lib -debug -nologo \#-subsystem:console \
            ../thirdParty/taglib-1.7.1/lib/$$BUILD/tag.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libcommon.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libbackend.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libexpand.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libbrass.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libchert.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libflint.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libinmemory.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libmulti.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libmatcher.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libnet.lib \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/liblanguages.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libapi.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libremote.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libunicode.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libweight.lib  \
            ../thirdParty/xapian-1.2.8/lib/$$BUILD/libqueryparser.lib
}
unix {
    LIBS += -lxapian /usr/lib/libtag.so
}

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











