
#include "clientconnection.h"
#include "messagehandler.h"
#include "sysutils.h"
ClientConnection::ClientConnection(QObject *parent) :
    QThread(parent),mSocket(new QTcpSocket(this)),mHostIp("127.0.0.1"),mPort(8080),mActive(false)
{
    //mName(getRandomName());
    mName=SysUtils::getUserName();
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(onRead()),Qt::QueuedConnection);
    connect(mSocket,SIGNAL(connected()),this,SLOT(onConnected()),Qt::QueuedConnection);
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()),Qt::QueuedConnection);
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
void ClientConnection::connectNow()
{
    mSocket->connectToHost(mHostIp,mPort);
}

void ClientConnection::run()
{
    exec();
    return;
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

void ClientConnection::disconnectFromServer()
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
    Message readmessage(mSocket->readAll());
    switch (readmessage.type()) {
    case Message::Invalid:
        break;
    case Message::Online:
        //qDebug()<<"Logon"<<readmessage.buddies();
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::LogOff:
        //Log off Notification from Other Client, Lets update the latest buddies
        qDebug()<<"Logoff"<<readmessage.buddies();
        mBuddies=readmessage.buddies();
        emit newMessage(readmessage.message(),"Server");
        emit buddylist(readmessage.message());
        break;
    case Message::Chat:
        qDebug()<<mName<<"> "<<readmessage.sender()<<"::"<<readmessage.message();
        if(readmessage.sender()==mName)
        {
            emit newMessage(readmessage.message(),"You",false);
        }
        else
        {
            emit newMessage(readmessage.message(),readmessage.sender(),false);
        }
    break;
    case Message::Mention:
        if(readmessage.sender()==mName)
        {
            emit newMessage(readmessage.message(),"You",true);
        }
        else
        {
            emit newMessage(readmessage.message(),readmessage.sender(),true);
        }
    break;
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
    if(!mSocket->waitForBytesWritten())
    {
        qCritical()<<"Unable to Wait for Data Sending";
    }
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
    //mSocket->deleteLater();
}

