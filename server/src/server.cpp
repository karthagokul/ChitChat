#include "server.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include "chatroom.h"
#include "chatsession.h"
#include "discoveryhandler.h"

Server::Server(QObject *aParent)
    :QObject(aParent),mChatRoom(0)
{
    mSocketServer=new QTcpServer(this);
    connect(mSocketServer,SIGNAL(newConnection()),this,SLOT(onNewTCPConnection()));
#ifdef ENABLE_WEBSOCKETS
    mWebSocketServer=new QWebSocketServer(QStringLiteral("ChitChat Server"),
                                          QWebSocketServer::NonSecureMode, this);
    connect(mWebSocketServer,SIGNAL(newConnection()),this,SLOT(onNewWebConnection()));
#endif

#ifdef SEARCH_SERVER
    mSearchHandler=new DiscoveryManager(this);
    connect(mSearchHandler,SIGNAL(search()),this,SLOT(onSearch()));
    //not a client, hence false
#endif

}

#ifdef SEARCH_SERVER
void Server::onSearch()
{
    mSearchHandler->sendMyIdentity();
}
#endif

Server::~Server()
{
    if(mChatRoom)
    {
        mChatRoom->deleteLater();
    }
    if(mSocketServer)
    {
        mSocketServer->deleteLater();
    }
#ifdef ENABLE_WEBSOCKETS
    if(mWebSocketServer)
    {
        mWebSocketServer->deleteLater();
    }
#endif
}

bool Server::init()
{
    if(mChatRoom)
    {
        qCritical()<<"Chat Room is Already Active";
        return false;
    }
    mChatRoom=new ChatRoom(this);

    mSocketServer->listen(QHostAddress::Any,tcp_port);
    qDebug()<<"TCP Server Listening on"<<mSocketServer->serverAddress()<<":"<<mSocketServer->serverPort();
#ifdef ENABLE_WEBSOCKETS
    mWebSocketServer->listen(QHostAddress::Any,web_port);
    qDebug()<<"WebSocketServer Listening on"<<mWebSocketServer->serverAddress()<<":"<<mWebSocketServer->serverPort();
#endif
    return true;
}

void Server::onNewTCPConnection()
{
    qDebug()<<"Got a New TCP    Connection";
    QTcpSocket *socket=mSocketServer->nextPendingConnection();
    ChatSession *session=new ChatSession(socket);
    mChatRoom->registerSession(session);
}

#ifdef ENABLE_WEBSOCKETS
void Server::onNewWebConnection()
{
    qDebug()<<"Got a New WebConnection";
    QWebSocket* socket=mWebSocketServer->nextPendingConnection();
    ChatSession *session=new ChatSession(socket);
    mChatRoom->registerSession(session);
}
#endif
