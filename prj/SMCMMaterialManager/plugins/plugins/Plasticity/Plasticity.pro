QT       += core gui opengl

TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 plugin

TARGET = Plasticity

INCLUDEPATH += ../../common/src \
               ../../../../../src/

include(         ../../common/prj/SMCMMMPlugins.pri        )

SOURCES += \
    Dialog.cpp

HEADERS += \
    Dialog.h

FORMS += \
    Dialog.ui

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

CONFIG -= debug_and_release

CONFIG(debug, debug|release) {
    # debug
    unix: {
        macx: {
#            QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/usr/local/include
#            QMAKE_LFLAGS += -lomp
#            LIBS += -L /usr/local/lib /usr/local/lib/libomp.dylib
        } else {
#            QMAKE_LFLAGS = -Wl,--no-as-needed -ldl -fopenmp -no-pie
#            QMAKE_CXXFLAGS += -march=sandybridge -mtune=cascadelake -m64 -flto -Og -g -fopenmp
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
#        QMAKE_LFLAGS = -Wl,--no-as-needed -ldl -fopenmp
#        QMAKE_CXXFLAGS += -march=sandybridge -mtune=cascadelake -m64 -flto -O3 -fopenmp
    } else {
#        QMAKE_LFLAGS = -Wl,--no-as-needed -fopenmp
#        QMAKE_CXXFLAGS += -Ofast -flto -funroll-loops -fopenmp
    }

    OBJECTS_DIR = ../../bin/$$TARGET/release/obj/$$TARGET
    MOC_DIR     = ../../bin/$$TARGET/release/moc/$$TARGET
    RCC_DIR     = ../../bin/$$TARGET/release/rcc/$$TARGET
}
