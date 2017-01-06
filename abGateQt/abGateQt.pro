#-------------------------------------------------
#
# Project created by QtCreator 2012-04-14T15:06:53
#
#-------------------------------------------------

QT       += core gui

CONFIG += shared plugin

TARGET = abGateQt
TEMPLATE = lib

QMAKE_CXXFLAGS += $(CPPFLAGS) -g
QMAKE_LFLAGS += $(LDFLAGS)

SOURCES +=\
        mainwindow.cpp \
    abgatestyle.cpp \
    gate_gui_Qt4.cpp

HEADERS  += mainwindow.h \
    abgatestyle.h \
    ui.h \
    gate_const.h

FORMS    += mainwindow.ui

RESOURCES += \
    resursas.qrc
