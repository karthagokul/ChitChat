#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T19:15:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGUI_Client
TEMPLATE = app
COMMON_PATH=../../common/src

INCLUDEPATH+=$${COMMON_PATH}

CONFIG+=multicastsupport

multicastsupport {
    message("Server Discovery : Enabled")
    SOURCES += $${COMMON_PATH}/discoveryhandler.cpp
    HEADERS +=$${COMMON_PATH}/discoveryhandler.h
    DEFINES+=SEARCH_SERVER
}else {
    message("Server Discovery : Disabled")
}


SOURCES += main.cpp\
        widget.cpp \
    $${COMMON_PATH}/messagehandler.cpp \
    serverlogindialog.cpp \
    clientconnection.cpp \
    mainwindow.cpp

HEADERS  += widget.h \
    $${COMMON_PATH}/messagehandler.h \
    serverlogindialog.h \
    clientconnection.h \
    mainwindow.h

FORMS    += widget.ui \
    serverlogindialog.ui \
    mainwindow.ui
