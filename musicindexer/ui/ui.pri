QT      += gui

INCLUDEPATH += ../

HEADERS += \
    ui/mainwindow.h \
    ui/musicviewer.h \
    ui/collectionview.h \
    ui/artistdetail.h \
    ui/similarityview.h \
    ui/swidget.h \
    ui/similarity.h \
    ui/albuminfo.h



SOURCES += \
    ui/mainwindow.cpp \
    ui/main.cpp \
    ui/musicviewer.cpp \
    ui/collectionview.cpp \
    ui/artistdetail.cpp \
    ui/similarityview.cpp \
    ui/swidget.cpp \
    ui/similarity.cpp \
    ui/albuminfo.cpp



FORMS += \
    ui/collectionview.ui \
    ui/mainwindow.ui \
    ui/artistdetail.ui \
    ui/similarityview.ui \
    ui/swidget.ui \
    ui/similarity.ui \
    ui/albuminfo.ui


RESOURCES += \
    resources/icons.qrc

OTHER_FILES += \
    resources/genres.xml \
    resources/config.conf
















