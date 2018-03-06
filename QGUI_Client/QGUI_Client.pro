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


SOURCES += main.cpp\
        widget.cpp \
    ../common/messagehandler.cpp \
    serverlogindialog.cpp \
    clientconnection.cpp

HEADERS  += widget.h \
    ../common/messagehandler.h \
    serverlogindialog.h \
    clientconnection.h

FORMS    += widget.ui \
    serverlogindialog.ui
