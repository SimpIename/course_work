QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 plugin

INCLUDEPATH += \
    ../../common/src \
    $$PWD/../../../../../src \
    $$PWD/../../../../../incl \
    $$PWD/../../../../../incl/Client

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=../../common/src/QXlsx/            # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=../../common/src/QXlsx/header/     # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=../../common/src/QXlsx/source/     # current QXlsx source path is ./source/
include(         ../../common/src/QXlsx/QXlsx.pri)

SOURCES += \
    ../../../../../src/Client/Shared/ChangePointDialog/ChangePointDialog.cpp \
    ../../../../../src/Client/Shared/Chart/ChartWindow/ChartWindow.cpp \
    ../../common/src/MyTableWidget/mytablewidget.cpp \
    ../../common/src/PluginAddDialog/pluginadddialog.cpp \
    ../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.cpp

HEADERS += \
    ../../../../../src/Client/Shared/ChangePointDialog/ChangePointDialog.h \
    ../../../../../src/Client/Shared/Chart/ChartWindow/ChartWindow.h \
    $$PWD/../../../../../src/Shared/plugin/interfaceTypes.h \
    ../../common/src/MyTableWidget/mytablewidget.h \
    ../../common/src/PluginAddDialog/pluginadddialog.h \
    ../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.h \
    ../../../../../src/Shared/plugin/interface.h

FORMS += \
    ../../common/src/PluginAddDialog/pluginadddialog.ui \
    ../../common/ui/mytablewidget.ui \
    ../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.ui

DESTDIR = ../../../../../bin/plugins/MaterialManager

RESOURCES += \
    ../../common/qrc/resources.qrc \
    ../../common/qrc/styles.qrc \
    ../../../../../qrc/SMCMChart/ChartWindowShaders.qrc

DISTFILES += \
    ../../common/qrc/resources/columnPlus.png \
    ../../common/qrc/resources/rowPlus.png


