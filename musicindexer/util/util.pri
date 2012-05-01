INCLUDEPATH += ../

win32 {
    INCLUDEPATH += ../thirdParty/taglib-1.7.1/include
}

HEADERS += \
    util/worker.h \
    util/QFileUtils.h \
    util/exception.h \
    util/distancecalculator.h \
    util/entitiesutil.h \
    util/MException.h

SOURCES += \
    util/QFileUtils.cpp \
    util/distancecalculator.cpp \
    util/entitiesutil.cpp









