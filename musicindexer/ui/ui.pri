QT      += gui

INCLUDEPATH += ../

HEADERS += \
    ui/mainwindow.h \
    ui/musicviewer.h \
    ui/collectionview.h \
    ui/artistdetail.h \
    ui/similarityview.h


SOURCES += \
    ui/mainwindow.cpp \
    ui/main.cpp \
    ui/musicviewer.cpp \
    ui/collectionview.cpp \
    ui/artistdetail.cpp \
    ui/similarityview.cpp


FORMS += \
    ui/collectionview.ui \
    ui/mainwindow.ui \
    ui/artistdetail.ui \
    ui/similarityview.ui

RESOURCES += \
    resources/icons.qrc

OTHER_FILES += \
    resources/genres.xml \
    resources/config.conf














