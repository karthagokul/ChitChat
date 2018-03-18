#include "chatsession.h"
#include "messagehandler.h"
#include "chatroom.h"
#include <QtNetwork>
#include <QUuid>

#ifdef ENABLE_WEBSOCKETS
ChatSession::ChatSession(QWebSocket *aSocket,QObject *aParent) :
    QThread(aParent),mWebSocket(aSocket),mType(WebSocketClient)
{
    mId=QUuid::createUuid().toString();
    connect(mWebSocket,SIGNAL(textMessageReceived(QString)),this,SLOT(onWebSocketRead(QString)), Qt::QueuedConnection);
    connect(mWebSocket,SIGNAL(disconnected()),this,SLOT(disconnected()), Qt::QueuedConnection);
}
#endif

ChatSession::ChatSession(QTcpSocket *aSocket, QObject *aParent):
    QThread(aParent),mSocket(aSocket),mType(TCPSocketClient)
{
    mId=QUuid::createUuid().toString();
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::QueuedConnection);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()),Qt::QueuedConnection);
}

ChatSession:: ~ChatSession()
{
}

void ChatSession::run()
{
    //qDebug()<<Q_FUNC_INFO;
    exec();
}

#ifdef ENABLE_WEBSOCKETS
void ChatSession::onWebSocketRead(const QString &message)
{
    //qDebug()<<"On Message"<<message;
    if(mType==WebSocketClient)
    {
        QByteArray data=message.toUtf8();
        onClientRead(data);
    }
}
#endif

void ChatSession::readyRead()
{
    QByteArray data=mSocket->readAll();
    onClientRead(data);
}

void ChatSession::onClientRead(QByteArray data)
{
    //qDebug()<<data;
    Message newMessage(data);
    switch (newMessage.type())
    {
    case Message::Invalid:
        //qDebug()<<"Unsupported Message Type";
        break;
    case Message::LogOn:
        //qDebug()<<"New Login : " <<newMessage.sender();
        mName=newMessage.sender();
        emit loggedin(newMessage,id());
        break;
    case Message::Chat:
        //qDebug()<<"chat";
        emit newmessage(newMessage,id());
        break;
    case Message::Mention:
       // qDebug()<<"Private Message";
        emit newmessage(newMessage,id());
    default:
        break;
    }

}

void ChatSession::write(QByteArray aData)
{
    if(mType==TCPSocketClient)
    {
        mSocket->write(aData);
        mSocket->waitForBytesWritten();
    }
#ifdef ENABLE_WEBSOCKETS
    if(mType==WebSocketClient)
    {
        mWebSocket->sendTextMessage(QString::fromUtf8(aData));
        //qDebug()<<"WebClient";
    }
#endif
}

void ChatSession::disconnected()
{
    //qDebug()<<"Disconnected";
    emit disconnecting(id());
    if(mType==TCPSocketClient)
    {
        mSocket->deleteLater();
    }
#ifdef ENABLE_WEBSOCKETS
    if(mType==WebSocketClient)
    {
        mWebSocket->deleteLater();
    }
#endif
    exit(0);
}

