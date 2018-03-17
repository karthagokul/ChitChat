#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T19:15:43
#
#-------------------------------------------------

TARGET = cc-client
TEMPLATE = app

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

COMMON_PATH=../../common/src

INCLUDEPATH+=$${COMMON_PATH}

CONFIG+=multicastsupport

multicastsupport {
    message("Server Discovery : Enabled")
  DEFINES+=SEARCH_SERVER
}else {
    message("Server Discovery : Disabled")
}


SOURCES += main.cpp\
        widget.cpp \
    $${COMMON_PATH}/messagehandler.cpp \
    serverlogindialog.cpp \
    clientconnection.cpp \
    mainwindow.cpp \
    sysutils.cpp \
    chatinputtext.cpp \
    emotiselectorwidget.cpp

HEADERS  += widget.h \
    $${COMMON_PATH}/messagehandler.h \
    serverlogindialog.h \
    clientconnection.h \
    mainwindow.h \
    sysutils.h \
    chatinputtext.h \
    emotiselectorwidget.h

FORMS    += \
    serverlogindialog.ui \
    mainwindow.ui \
    widget.ui

RESOURCES += \
    client.qrc
