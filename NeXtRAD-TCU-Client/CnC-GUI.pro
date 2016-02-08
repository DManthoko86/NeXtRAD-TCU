#-------------------------------------------------
#
# Project created by QtCreator 2016-01-16T14:35:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CnC-GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rhino.cpp \
    inih/cpp/INIReader.cpp \
    tcpsocket/tcpconnector.cpp \
    tcpsocket/tcpstream.cpp \
    inih/ini.c \
    backend.cpp \
    rhinogui.cpp \
    experimentgui.cpp

HEADERS  += mainwindow.h \
    rhino.h \
    inih/cpp/INIReader.h \
    inih/ini.h \
    tcpsocket/tcpconnector.h \
    tcpsocket/tcpstream.h \
    backend.h \
    rhinogui.h \
    experimentgui.h

FORMS    += mainwindow.ui \
    rhinogui.ui \
    experimentgui.ui

DISTFILES +=
