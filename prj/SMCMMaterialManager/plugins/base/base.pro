QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += ../common/src \
               ../../../../src/Shared/plugin/

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../../../src/Shared/plugin/interfaceLoader.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = ../../../../bin


CONFIG -= debug_and_release

CONFIG(debug, debug|release) {
    # debug

    OBJECTS_DIR = ../bin/$$TARGET/debug/obj/$$TARGET
    MOC_DIR     = ../bin/$$TARGET/debug/moc/$$TARGET
    RCC_DIR     = ../bin/$$TARGET/debug/rcc/$$TARGET
} else {
    # release

    OBJECTS_DIR = ../bin/$$TARGET/release/obj/$$TARGET
    MOC_DIR     = ../bin/$$TARGET/release/moc/$$TARGET
    RCC_DIR     = ../bin/$$TARGET/release/rcc/$$TARGET
}

unix: {
    macx: {
        DEFINES += PLUGIN_DIR='"\\\"../../../plugins/MaterialManager\\\""'
    } else {
        DEFINES += PLUGIN_DIR='"\\\"/plugins/MaterialManager\\\""'
    }
} else {
        DEFINES += PLUGIN_DIR='"\\\"/plugins/MaterialManager\\\""'
    }
