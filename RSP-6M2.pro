#-------------------------------------------------
#
# Project created by QtCreator 2014-07-23T10:32:45
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

TARGET = RSP-6M2
TEMPLATE = app


SOURCES += main.cpp\
        menu.cpp \
    indicator.cpp \
    drl.cpp \
    drlview.cpp

HEADERS  += menu.h \
    indicator.h \
    drl.h \
    drlview.h

FORMS    += menu.ui \
    drlview.ui

RESOURCES += \
    common.qrc
