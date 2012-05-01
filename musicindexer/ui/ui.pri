QT      += gui phonon

INCLUDEPATH += ../

HEADERS += \
    ui/mainwindow.h \
    ui/musicviewer.h \
    ui/collectionview.h \
    ui/artistdetail.h \
    ui/similarityview.h \
    ui/swidget.h \
    ui/similarity.h \
    ui/albuminfo.h \
    ui/compactplayer.h \
    ui/compactplaylist.h \
    ui/fullscreenview.h \
    ui/importview.h \
    ui/qtwin.h \
    ui/musicview.h

SOURCES += \
    ui/mainwindow.cpp \
    ui/main.cpp \
    ui/musicviewer.cpp \
    ui/collectionview.cpp \
    ui/artistdetail.cpp \
    ui/similarityview.cpp \
    ui/swidget.cpp \
    ui/similarity.cpp \
    ui/albuminfo.cpp \
    ui/compactplayer.cpp \
    ui/compactplaylist.cpp \
    ui/fullscreenview.cpp \
    ui/importview.cpp \
    ui/qtwin.cpp \
    ui/musicview.cpp

FORMS += \
    ui/collectionview.ui \
    ui/mainwindow.ui \
    ui/artistdetail.ui \
    ui/similarityview.ui \
    ui/swidget.ui \
    ui/similarity.ui \
    ui/albuminfo.ui \
    ui/compactplayer.ui \
    ui/compactplaylist.ui \
    ui/fullscreenview.ui \
    ui/importview.ui \
    ui/musicview.ui


RESOURCES += \
    resources/icons.qrc

OTHER_FILES += \
    resources/genres.xml \
    resources/config.conf






















