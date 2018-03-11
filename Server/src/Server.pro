QT -= gui

CONFIG+=websocketsupport multicastsupport

COMMON_PATH=../../common/src

websocketsupport {
    message("Websocket Support :Enabled")
    QT+=websockets
    DEFINES+=ENABLE_WEBSOCKETS
} else {
    message("Websocket Support : Disabled")
}

multicastsupport {
    message("Server Discovery : Enabled")
    SOURCES += $${COMMON_PATH}/discoveryhandler.cpp
    HEADERS += $${COMMON_PATH}/discoveryhandler.h
    DEFINES+=SEARCH_SERVER
}else {
    message("Server Discovery : Disabled")
}

QT +=network

CONFIG += c++11 console
CONFIG -= app_bundle
INCLUDEPATH+=$${COMMON_PATH}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    server.cpp \
    serverapplication.cpp \
    chatroom.cpp \
    $${COMMON_PATH}/messagehandler.cpp \
    chatsession.cpp

HEADERS += \
    server.h \
    serverapplication.h \
    chatroom.h \
    $${COMMON_PATH}/messagehandler.h \
    chatsession.h
