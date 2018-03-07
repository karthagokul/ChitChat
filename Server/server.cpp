#include "server.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include "chatroom.h"

Server::Server(QObject *aParent)
    :QTcpServer(aParent),mChatRoom(0)
{

}

Server::~Server()
{
    mChatRoom->closeAllSessions();
    delete mChatRoom;
    mChatRoom=0;
}

bool Server::init()
{
    if(mChatRoom)
    {
        qCritical()<<"Chat Room is Already Active";
        return false;
    }
    mChatRoom=new ChatRoom(this);
    listen(QHostAddress::Any,server_port);
    qDebug()<<"Server Has Been Started on "<<serverAddress()<<":"<<serverPort();
    return true;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    mChatRoom->createNewSession(socketDescriptor);
}

