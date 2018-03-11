#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T19:15:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGUI_Client
TEMPLATE = app
INCLUDEPATH +=../common

CONFIG+=multicastsupport

multicastsupport {
    message("Server Discovery : Enabled")
    SOURCES += ../common/discoveryhandler.cpp
    HEADERS +=../common/discoveryhandler.h
    DEFINES+=SEARCH_SERVER
}else {
    message("Server Discovery : Disabled")
}


SOURCES += main.cpp\
        widget.cpp \
    ../common/messagehandler.cpp \
    serverlogindialog.cpp \
    clientconnection.cpp \
    mainwindow.cpp

HEADERS  += widget.h \
    ../common/messagehandler.h \
    serverlogindialog.h \
    clientconnection.h \
    mainwindow.h

FORMS    += widget.ui \
    serverlogindialog.ui \
    mainwindow.ui
