
#include "clientconnection.h"
#include "messagehandler.h"
#include "sysutils.h"
ClientConnection::ClientConnection(QObject *parent) :
    QObject(parent),mSocket(new QTcpSocket(this)),mHostIp("127.0.0.1"),mPort(8080),mActive(false)
{
    //mName(getRandomName());
    mName=SysUtils::getUserName();
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(onRead()));
    connect(mSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(mSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
}

void ClientConnection::setServer(QString aHostName,int aPort)
{
    mHostIp=aHostName;
    mPort=aPort;
}

void ClientConnection::setUserName(QString aName)
{
    mName=aName;
}

void ClientConnection::onConnected()
{
    mActive=true;
    Message loginmessage(Message::LogOn,mName,QString(),QStringList());
    mSocket->write(loginmessage.toByteArray());
    emit stateChanged();
}

void ClientConnection::onDisconnected()
{
    mActive=false;
    emit stateChanged();
}

void ClientConnection::start()
{
    qDebug()<<"Connecting to Server";
    if(isActive())
    {
        qDebug()<<"Already Have an Active Session";
        return;
    }
    mSocket->connectToHost(mHostIp,mPort);
}

void ClientConnection::onError(QAbstractSocket::SocketError aError)
{
    qDebug()<<aError;

    switch(aError)
    {
    case QAbstractSocket::ConnectionRefusedError:
        emit error("Connected Refused to "+mHostIp);
        break;
    case QAbstractSocket::RemoteHostClosedError:
        emit error("Server Went Down at "+mHostIp);
        break;
    default:
        emit error("Unknown Error");
        break;
    }

    emit stateChanged();
}

void ClientConnection::stop()
{
    qDebug()<<"Disconnecting from Server";
    if(!isActive())
    {
        qDebug()<<"Do not have Active Session";
        return;
    }
    mSocket->disconnectFromHost();
    mSocket->close();
    mBuddies.clear();
    emit stateChanged();
    emit newMessage("Logged Out from Chat Server !","You");

}

void ClientConnection::onRead()
{
    //qDebug()<<"On Server Message";
    Message readmessage(mSocket->readAll());
    switch (readmessage.type()) {
    case Message::Invalid:
        break;
    case Message::Online:
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::LogOff:
        //Log off Notification from Other Client, Lets update the latest buddies
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::Chat:
    case Message::Mention:
        if(readmessage.sender()==mName)
        {
            emit newMessage(readmessage.message(),"You");
        }
        else
        {
            emit newMessage(readmessage.message(),readmessage.sender());
        }

    default:
        break;
    }
}

void ClientConnection::send(const QString &aData)
{
    if(!isActive())
    {
        qCritical()<<"No Active Connection to send Message";
        return;
    }

    Message newMessage(Message::Chat,mName,aData,QStringList());
    mSocket->write(newMessage.toByteArray());
}

void ClientConnection::sendToSelected(const QString &aData,const QStringList &aSelectedBuddies)
{
    if(!isActive())
    {
        qCritical()<<"No Active Connection to send Message";
        return;
    }

    Message newMessage(Message::Mention,mName,aData,aSelectedBuddies);
    mSocket->write(newMessage.toByteArray());
}

ClientConnection::~ClientConnection()
{
    if(mSocket)
    {
        delete mSocket;
        mSocket=0;
    }
}

