#-------------------------------------------------
#
# Project created by QtCreator 2018-03-06T19:15:43
#
#-------------------------------------------------

TARGET = client-simulator
TEMPLATE = app

QT       += core  network
QT -= gui
CONFIG += c++11 console
CONFIG -= app_bundle

COMMON_PATH=../common/src

INCLUDEPATH+=$${COMMON_PATH} ../desktop-client/src/

SOURCES += main.cpp\
    $${COMMON_PATH}/messagehandler.cpp \
    ../desktop-client/src/clientconnection.cpp \
    ../desktop-client/src/sysutils.cpp \
    simulatorthread.cpp \
    simulatorapp.cpp

HEADERS  += $${COMMON_PATH}/messagehandler.h \
    ../desktop-client/src/clientconnection.h \
    ../desktop-client/src/sysutils.h \
    simulatorthread.h \
    simulatorapp.h
