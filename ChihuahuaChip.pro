QT += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    emulator.cpp \
    cpu.cpp

HEADERS += \
    emulator.hpp \
    cpu.hpp
