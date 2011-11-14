#-------------------------------------------------
#
# Project created by QtCreator 2011-10-06T19:44:31
#
#-------------------------------------------------

QT       += core

TARGET   =  musicIndexer
CONFIG   += order

TEMPLATE = app

OBJECTS_DIR = .obj/
MOC_DIR     = .moc/

INCLUDEPATH += .

include (logging/logging.pri)
include (configuration/configuration.pri)
include (util/util.pri)
include (ui/ui.pri)
include (controller/controller.pri)
include (entities/entities.pri)
include (database/database.pri)









