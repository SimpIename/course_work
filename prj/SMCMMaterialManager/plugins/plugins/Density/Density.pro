QT += widgets opengl

TEMPLATE = lib

CONFIG += c++11 plugin

TARGET = Density

INCLUDEPATH += ../../common/src \
               ../../../../../incl \
               ../../../../../incl/Client \
               ../../../../../src/Shared/plugin

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=../../common/src/QXlsx/            # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=../../common/src/QXlsx/header/     # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=../../common/src/QXlsx/source/     # current QXlsx source path is ./source/
include(         ../../common/src/QXlsx/QXlsx.pri)

SOURCES += \
    ../../common/src/MyTableWidget/mytablewidget.cpp \
    ../../common/src/PluginAddDialog/pluginadddialog.cpp \
    ../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.cpp \
    DensityDialog.cpp \
    calculatorDensity.cpp

HEADERS += \
    ../../common/src/MyTableWidget/mytablewidget.h \
    ../../common/src/PluginAddDialog/pluginadddialog.h \
    ../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.h \
    ../../../../../src/Shared/plugin/interface.h \
    DensityDialog.h \
    calculatorDensity.h

FORMS += \
    ../../common/src/PluginAddDialog/pluginadddialog.ui \
    ../../common/src/PluginBaseElementsWidget/PluginBaseElementsWidget.ui \
    ../../common/ui/mytablewidget.ui \
    DensityDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(DEFINES, IFW_PACKAGES) {
    DESTDIR = ../../../../SMCMInstaller/SMCMInstaller/packages/ru.simplex.manipula.materialmanager.plugins/data/plugins/MaterialManager
      
    copymeta.commands = cp -r \
                        ../../../../SMCMInstaller/SMCMInstaller/packages_base/ru.simplex.manipula.materialmanager.plugins \
                        ../../../../SMCMInstaller/SMCMInstaller/packages/ $$escape_expand(\n\t)

    first.depends = $(first) copymeta
    export(first.depends)
    export(copymeta.commands)
    QMAKE_EXTRA_TARGETS += first copymeta
}
else {
    DESTDIR = ../../../../../bin/plugins/MaterialManager
}

RESOURCES += \
    ../../common/qrc/resources.qrc \
    ../../common/qrc/styles.qrc

DISTFILES += \
    ../../common/qrc/resources/columnPlus.png \
    ../../common/qrc/resources/rowPlus.png

CONFIG -= debug_and_release

CONFIG(debug, debug|release) {
    # debug
    unix: {
        macx: {
            QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/usr/local/include
            QMAKE_LFLAGS += -lomp
            LIBS += -L /usr/local/lib /usr/local/lib/libomp.dylib
        } else {
            QMAKE_LFLAGS = -Wl,--no-as-needed -ldl -fopenmp -no-pie
            QMAKE_CXXFLAGS += -march=sandybridge -mtune=cascadelake -m64 -flto -Og -g -fopenmp
        }
    } else {
        QMAKE_LFLAGS = -fopenmp
        QMAKE_CXXFLAGS += -fopenmp
    }

    OBJECTS_DIR = ../../bin/$$TARGET/debug/obj/$$TARGET
    MOC_DIR     = ../../bin/$$TARGET/debug/moc/$$TARGET
    RCC_DIR     = ../../bin/$$TARGET/debug/rcc/$$TARGET
} else {
    # release
    unix: {
        QMAKE_LFLAGS = -Wl,--no-as-needed -ldl -fopenmp
        QMAKE_CXXFLAGS += -march=sandybridge -mtune=cascadelake -m64 -flto -O3 -fopenmp
    } else {
        QMAKE_LFLAGS = -Wl,--no-as-needed -fopenmp
        QMAKE_CXXFLAGS += -Ofast -flto -funroll-loops -fopenmp
    }

    OBJECTS_DIR = ../../bin/$$TARGET/release/obj/$$TARGET
    MOC_DIR     = ../../bin/$$TARGET/release/moc/$$TARGET
    RCC_DIR     = ../../bin/$$TARGET/release/rcc/$$TARGET
}
