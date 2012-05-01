#-------------------------------------------------
#
# Project created by QtCreator 2011-10-06T19:44:31
#
#-------------------------------------------------

QT       += core

TARGET   =  muxer
CONFIG   += order


TEMPLATE = app

OBJECTS_DIR = .obj/
MOC_DIR     = .moc/
UI_DIR      = .ui/

INCLUDEPATH += .

include (logging/logging.pri)
include (configuration/configuration.pri)
include (util/util.pri)
include (ui/ui.pri)
include (controller/controller.pri)
include (entities/entities.pri)
include (database/database.pri)

unix {
    LIBS += -lxapian /usr/lib/libtag.so
}

win32 {
    INCLUDEPATH += ../thirdParty/taglib-1.7.1/include ../thirdParty/xapian-1.2.8/include

    CONFIG(release, debug|release)
    {
      message( "buidling in release" )
      BUILD = release
    }
    CONFIG(debug, debug|release)
    {
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

    QMAKE_CXXFLAGS += -DQT_NO_KEYWORDS

}











