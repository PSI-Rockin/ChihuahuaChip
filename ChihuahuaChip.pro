QT += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QMAKE_CFLAGS_RELEASE -= -O
QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE *= -O2
QMAKE_CFLAGS_RELEASE -= -O3

SOURCES += main.cpp \
    emulator.cpp \
    cpu.cpp \
    displaywindow.cpp \
    jitcache.cpp \
    emitterx64.cpp

HEADERS += \
    emulator.hpp \
    cpu.hpp \
    displaywindow.hpp \
    jitcache.hpp \
    emitterx64.hpp
